// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_PickUpComponent.h"

AActor* OtherActorTest;

UTP_PickUpComponent::UTP_PickUpComponent()
{
	// Setup the Sphere Collision
	SphereRadius = 32.f;
}

void UTP_PickUpComponent::pickWeaponUp()
{
	ADES310_MaydayGamesCharacter* Character = Cast<ADES310_MaydayGamesCharacter>(OtherActorTest);

	// Notify that the actor is being picked up
	OnPickUp.Broadcast(Character);

	// Unregister from the Overlap Event so it is no longer triggered
	OnComponentBeginOverlap.RemoveAll(this);
}

void UTP_PickUpComponent::BeginPlay()
{
	Super::BeginPlay();

	// Register our Overlap Event
	OnComponentBeginOverlap.AddDynamic(this, &UTP_PickUpComponent::OnSphereBeginOverlap);
}

void UTP_PickUpComponent::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checking if it is a First Person Character overlapping
	ADES310_MaydayGamesCharacter* Character = Cast<ADES310_MaydayGamesCharacter>(OtherActor);
	OtherActorTest = OtherActor;
	if(Character != nullptr)
	{
		// Set up action bindings
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				// Fire
				EnhancedInputComponent->BindAction(PickupAction, ETriggerEvent::Triggered, this, &UTP_PickUpComponent::pickWeaponUp);

				
			}
			
		}
	}
}
