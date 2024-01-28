// Fill out your copyright notice in the Description page of Project Settings.


#include "playerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AplayerCharacter::AplayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	// Character doesnt have a rifle at start
	bHasRifle = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void AplayerCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AplayerCharacter::GetHasRifle()
{
	return bHasRifle;
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
	PlayerInputComponent->BindAction(TEXT("jump"), IE_Pressed, this, &AplayerCharacter::Jump);

}

