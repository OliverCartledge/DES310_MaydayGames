// Copyright Epic Games, Inc. All Rights Reserved.


#include "TP_WeaponComponent.h"
#include "Engine.h"
#include "DES310_MaydayGamesCharacter.h"
#include "DES310_MaydayGamesProjectile.h"
#include "GameFramework/PlayerController.h"
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

void UTP_WeaponComponent::Fire()
{
	if (Character == nullptr || Character->GetController() == nullptr)
	{
		return;
	}
	
	//If the player is hodling right click allow them to shoot
	if (IsADS)
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

		bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, SpawnLocation, EndPoint, ECC_Pawn, CollisionParams);

		if (bHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Thing hit: %s"), *OutHit.GetActor()->GetName()));
			ACPP_Enemy* enemyHit = Cast<ACPP_Enemy>(OutHit.GetActor());

			if (enemyHit != nullptr && enemyHit->ActorHasTag("Enemy"))
			{
				enemyHit->Destroy();
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
