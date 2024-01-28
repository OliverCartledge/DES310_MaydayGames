// Fill out your copyright notice in the Description page of Project Settings.


#include "playerCharacter.h"

// Sets default values
AplayerCharacter::AplayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AplayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AplayerCharacter::moveRight(float Axis)
{
	AddMovementInput(GetActorRightVector() * Axis);
}

void AplayerCharacter::moveForward(float Axis)
{
	AddMovementInput(GetActorForwardVector() * Axis);
}

// Called every frame
void AplayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AplayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("moveForward"), this, &AplayerCharacter::moveForward);
	PlayerInputComponent->BindAxis(TEXT("moveRight"), this, &AplayerCharacter::moveRight);
	PlayerInputComponent->BindAxis(TEXT("turnRight"), this, &AplayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("lookUp"), this, &AplayerCharacter::AddControllerPitchInput);

}

