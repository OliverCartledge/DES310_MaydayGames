// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "Engine.h"
#include "DES310_MaydayGamesCharacter.h"
#include "DES310_MaydayGamesProjectile.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Actor.h"
#include "Camera/PlayerCameraManager.h"
//#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CPP_Enemy.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	//particle system
	//ParticleSystem = CreateDefaultSubobject<UParticleSystemComponent>("ParticleSystem");
}

//Check if right click is being held down
void UTP_WeaponComponent::ADSPressed()
{
	IsADS = true;
}

//Check if right click has been released
void UTP_WeaponComponent::ADSReleased()
{
	IsADS = false;
}

//void UTP_WeaponComponent::winScreen()
//{
//
//}

void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Thing hit: %f"), bulletCount));

	//If the player is hodling right click allow them to shoot
	if (IsADS)
	{

		if (bulletCount >= 0 && canFire)
		{
			//particle system (to be tested and polished)
			//ParticleSystem->Activate();

			FHitResult OutHit;

			APlayerCameraManager* OurCamera = UGameplayStatics::GetPlayerCameraManager(this, 0);

			FVector ForwardVector = OurCamera->GetActorForwardVector();
			FRotator StartPoint = OurCamera->GetCameraRotation();
			const FVector SpawnLocation = GetOwner()->GetActorLocation() + StartPoint.RotateVector(MuzzleOffset);
			FVector EndPoint = SpawnLocation + (ForwardVector * 10000);
			FCollisionQueryParams CollisionParams;

			//ParticleSystem->SetWorldLocation(SpawnLocation);
			//ParticleSystem->SetWorldRotation(StartPoint);

			//Debug line for bug testing the gun fire 
			DrawDebugLine(GetWorld(), SpawnLocation, EndPoint, FColor::Green, true);

			canFire = false;
			StartTimer();
			
			bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, SpawnLocation, EndPoint, ECC_Pawn, CollisionParams);

			if (bHit)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Thing hit: %s"), *OutHit.GetActor()->GetName()));
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
						enemyHit->Destroy();
						playerScore += enemyHit->enemyGiveScore;
						showScore();

						if (playerScore >= 40)
						{
							//this is being met, as per tested by the debug message
							//but the winscreena nd bool aint showing in the BPs
							//so struggling ot set a win screen


							winScreen();
							//GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::White, FString::Printf(TEXT("WIN CONDITION MET"))); //DEBUG MSG
						}
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

			bulletCount -= 1;
		}
		else
			bulletCount = 10;

		//ParticleSystem->Deactivate();
	}
}

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
			EnhancedInputComponent->BindAction(SecondaryFireAction, ETriggerEvent::Started, this, &UTP_WeaponComponent::ADSPressed);
			EnhancedInputComponent->BindAction(SecondaryFireAction, ETriggerEvent::Completed, this, &UTP_WeaponComponent::ADSReleased);

			//Input mapping for left click 
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UTP_WeaponComponent::Fire);
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


void UTP_WeaponComponent::showScore()
{
		GEngine->AddOnScreenDebugMessage(-1, 60.f, FColor::White, FString::Printf(TEXT("Score: %d"), playerScore));
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
		OwningActor->GetWorldTimerManager().SetTimer(GunFireRate, this, &UTP_WeaponComponent::TimerExpired,  0.5f, false);
	}
}





