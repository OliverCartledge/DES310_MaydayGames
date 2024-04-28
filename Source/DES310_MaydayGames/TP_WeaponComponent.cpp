// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "Engine.h"
#include "DES310_MaydayGamesCharacter.h"
#include "DES310_MaydayGamesProjectile.h"
#include "Public/PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_Enemy.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	grenadeFire = true;

	//particle system
	//ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
}

//Check if right click is being held down
void UTP_WeaponComponent::ADSPressed()
{
	if (!Character->GetJumpStatus())
	{
		IsADS = true;

		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
		ADES310_MaydayGamesCharacter* Player = Cast<ADES310_MaydayGamesCharacter>(PlayerPawn);
		Player->ShouldShowCrosshair.Broadcast(IsADS);

		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint_0")));
	}
	//AddLocalRotation(FRotator(0.0, 0.0, -15.0));

}

//Check if right click has been released
void UTP_WeaponComponent::ADSReleased()
{
	IsADS = false;

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	ADES310_MaydayGamesCharacter* Player = Cast<ADES310_MaydayGamesCharacter>(PlayerPawn);
	Player->ShouldShowCrosshair.Broadcast(IsADS);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

}

//Check if right click has been released
void UTP_WeaponComponent::Reload()
{
	AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(UGameplayStatics::GetPlayerState(this, 0));

	if (!IsReloading)
	{
		if (bulletCount != 40 || grenadeCount != 3)
		{
			bulletCount = 40;
			grenadeCount = 3;
			IsReloading = true;

			MyPlayerState->updateAmmoCount(bulletCount);
			MyPlayerState->updateGrenadeCount(grenadeCount);


			AActor* OwningActor = GetOwner();

			AddLocalRotation(FRotator(25.0, 0.0, 0.0));

			if (OwningActor)
			{
				OwningActor->GetWorldTimerManager().SetTimer(ReloadTimer, this, &UTP_WeaponComponent::StopReload, 3.0f, true);
			}
		}
	}

}

void UTP_WeaponComponent::StopReload()
{
	IsReloading = false;
	AActor* OwningActor = GetOwner();

	AddLocalRotation(FRotator(-25.0, 0.0, 0.0));

	if (OwningActor)
		OwningActor->GetWorldTimerManager().ClearTimer(ReloadTimer);


}

void UTP_WeaponComponent::StartShoot()
{
	IsShooting = true;

	AActor* OwningActor = GetOwner();

	if (OwningActor)
	{
		Fire();
		OwningActor->GetWorldTimerManager().SetTimer(ShootingTimer, this, &UTP_WeaponComponent::Fire, 0.08, true);
	}
}

void UTP_WeaponComponent::EndShoot() 
{
	IsShooting = false;

	AActor* OwningActor = GetOwner();

	if (OwningActor)
		OwningActor->GetWorldTimerManager().ClearTimer(ShootingTimer);
}

void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	//If the player is hodling right click allow them to shoot
	if (IsADS && IsShooting)
	{

		if (bulletCount > 0 && !IsReloading && canFire)
		{

			AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
			 
			FHitResult OutHit;

			APlayerCameraManager* OurCamera = UGameplayStatics::GetPlayerCameraManager(this, 0);

			FVector ForwardVector = OurCamera->GetActorForwardVector();
			FRotator StartPoint = OurCamera->GetCameraRotation();
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + StartPoint.RotateVector(FVector(0.0f, -20.0f, 50.0f));
			//const FVector SpawnLocation = GetOwner()->GetActorLocation() + StartPoint.RotateVector(MuzzleOffset);
			FVector EndPoint = SpawnLocation + (ForwardVector * 10000);
			FCollisionQueryParams CollisionParams;
			
			canFire = false;
			StartTimer();

			//DrawDebugLine(GetWorld(), SpawnLocation, EndPoint, FColor::Green, true);

			bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, SpawnLocation, EndPoint, ECC_Pawn, CollisionParams);

			if (bHit)
			{
				ACPP_Enemy* enemyHit = Cast<ACPP_Enemy>(OutHit.GetActor());

				
				//if we hit an enemy
				if (enemyHit != nullptr && enemyHit->ActorHasTag("Enemy"))
				{

					//get enemy health
					float currentHealth = enemyHit->enemyHealth;
					currentHealth -= weaponDamage; //apply damage

					//if enemy has no health left, destroy the actor
					if (currentHealth <= 0)
					{
						if (enemyHit->EnemyDeathSound != nullptr)
						{
							UGameplayStatics::PlaySoundAtLocation(this, enemyHit->EnemyDeathSound, enemyHit->GetActorLocation());
						}
						enemyHit->Destroy();
						MyPlayerState->updateScore(enemyHit->enemyGiveScore);
					}
					//else, update health value
					else
					{
						enemyHit->enemyHealth = currentHealth;
					}
				}
			}
			
			// Try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
			}

			// Try and play a firing animation if specified
			if (FireAnimation != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(FireAnimation, 1.f);
				}
			}

			if (MuzzleFlash != nullptr)
			{
				const FVector MuzzlePoint = GetOwner()->GetActorLocation() + StartPoint.RotateVector(FVector(100.0f, 0.0f, 10.0f));
				UGameplayStatics::SpawnEmitterAtLocation(this, MuzzleFlash, MuzzlePoint);
			}

			if (bulletCount == 0)
			{
				Reload();
			}

			bulletCount -= 1;
			MyPlayerState->updateAmmoCount(bulletCount);
			//EndTimer();
		}
	}
}
  





//==============grenade launcher================================================================================================================================================



void UTP_WeaponComponent::GrenadeLauncher()
{
	AMyPlayerState* MyPlayerState = Cast<AMyPlayerState>(UGameplayStatics::GetPlayerState(this, 0));

	if (hasLauncher)
	{
		//grenadeCount = 3;
		//MyPlayerState->updateGrenadeCount(grenadeCount);

		if (IsADS && grenadeFire && grenadeCount > 0)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("AKLJ;HGJKDLS;HGKJLAPG")));
			// Try and fire a projectile
			if (ProjectileClass != nullptr)
			{

				UWorld* const World = GetWorld();
				if (World != nullptr)
				{
					APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
					const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
					// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
					const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

					//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Grenade launcher fired")));

					// Spawn the projectile at the muzzle
					World->SpawnActor<ADES310_MaydayGamesProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				}
			}
			grenadeCount -= 1;
			MyPlayerState->updateGrenadeCount(grenadeCount);

			grenadeFire = false;
#		
			//start the timer to create a fire rate of 1 shot every 2.5 seconds
			AActor* OwningActor = GetOwner();

			if (OwningActor)
			{
				OwningActor->GetWorldTimerManager().SetTimer(GrenadeLauncherDelay, this, &UTP_WeaponComponent::GrenadeLauncherDelayManager, .5f, false);
				//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Grenade launcher ready to fire")));
			}

			// Try and play the sound if specified
			if (LauncherFireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, LauncherFireSound, Character->GetActorLocation());
			}

			// Try and play a firing animation if specified
			if (FireAnimation != nullptr)
			{
				// Get the animation object for the arms mesh
				UAnimInstance* AnimInstance = Character->GetMesh1P()->GetAnimInstance();
				if (AnimInstance != nullptr)
				{
					AnimInstance->Montage_Play(FireAnimation, 1.f);
				}
			}
		}
	}
}

void UTP_WeaponComponent::GrenadeLauncherDelayManager()
{
	grenadeFire = true;
}

//=====================================================================================================================================================================================
bool UTP_WeaponComponent::LineTraceShot(FHitResult& OutHit)
{
	APlayerCameraManager* OurCamera = UGameplayStatics::GetPlayerCameraManager(this, 0);

	FVector ForwardVector = OurCamera->GetActorForwardVector();
	FVector StartPoint = OurCamera->GetCameraLocation();
	FVector EndPoint = StartPoint + (ForwardVector);

	FCollisionQueryParams Parameters;
	return GetWorld()->LineTraceSingleByChannel(OutHit, StartPoint, EndPoint, ECC_Visibility, Parameters);
}

void UTP_WeaponComponent::AttachWeapon(ADES310_MaydayGamesCharacter* TargetCharacter)
{
	Character = TargetCharacter;
	if (Character == nullptr)
	{
		return;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Character->GetMesh1P(), AttachmentRules, FName(TEXT("GripPoint")));

	// switch bHasRifle so the animation blueprint can switch to another animation set
	Character->SetHasRifle(true);

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			//Input mapping for holding/ releasing right click
			EnhancedInputComponent->BindAction(ADSAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::ADSPressed);
			EnhancedInputComponent->BindAction(ADSAction, ETriggerEvent::Completed, this, &UTP_WeaponComponent::ADSReleased);

			//Input mapping for left click 
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::StartShoot);
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UTP_WeaponComponent::EndShoot);

			//Input mapping for gun reload
			EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Reload);

			//if (hasLauncher)
			//{
			EnhancedInputComponent->BindAction(SecondaryFireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::GrenadeLauncher);
			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Grenade mapped")));
			//}
		}
	}
}

void UTP_WeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (Character == nullptr)
	{
		return;
	}

	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->RemoveMappingContext(FireMappingContext);
		}
	}
}

//commented out as this CANNOT be used in a skeletalmesh inherited class. Must be AACTOR so move this code.
void UTP_WeaponComponent::TimerExpired()
{
	canFire = true;
}

void UTP_WeaponComponent::StartTimer()
{
	// Start the timer to create a fire rate of -> 1 shot every x amount of time
	AActor* OwningActor = GetOwner();
	
	if (OwningActor)
	{
		OwningActor->GetWorldTimerManager().SetTimer(GunFireRate, this, &UTP_WeaponComponent::TimerExpired, 0.08, false);
	}
}

void UTP_WeaponComponent::EndTimer()
{
	// Start the timer to create a fire rate of -> 1 shot every x amount of time
	AActor* OwningActor = GetOwner();

	if (OwningActor)
	{
		OwningActor->GetWorldTimerManager().ClearTimer(GunFireRate);
	}
}
