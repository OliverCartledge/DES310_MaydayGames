// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Engine.h"
#include "CPP_Enemy.h"
#include "Camera/CameraComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerCharacterMesh"));
	PlayerMesh->SetOnlyOwnerSee(true);
	PlayerMesh->SetupAttachment(FirstPersonCameraComponent);
	PlayerMesh->bCastDynamicShadow = false;
	PlayerMesh->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	PlayerMesh->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	Tags.Add(FName("Player"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		//Crouch
		//EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::CrouchPressed);
		//EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacter::CrouchReleased);

		//Input mapping for holding/ releasing right click
		EnhancedInputComponent->BindAction(ADSAction, ETriggerEvent::Started, this, &APlayerCharacter::ADSPressed);
		EnhancedInputComponent->BindAction(ADSAction, ETriggerEvent::Completed, this, &APlayerCharacter::ADSReleased);
	}
}

void APlayerCharacter::ADSPressed()
{
	IsADS = true;
	Crouch();
}

void APlayerCharacter::ADSReleased()
{
	IsADS = false;
	UnCrouch();
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	if (!IsADS)
	{
		// input is a Vector2D
		FVector2D MovementVector = Value.Get<FVector2D>();

		if (Controller != nullptr)
		{
			// add movement 
			AddMovementInput(GetActorForwardVector(), MovementVector.Y);
			AddMovementInput(GetActorRightVector(), MovementVector.X);
		}
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool APlayerCharacter::GetHasRifle()
{
	return bHasRifle;
}

void APlayerCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(ACPP_Enemy::StaticClass()))
	{
		//Damage player using a timer similar to how the full auto / fire rate is set up. deadass took me 2 hours to get this working :)
		GetWorld()->GetTimerManager().SetTimer(DamageTimeHandle, this, &APlayerCharacter::DealDamage, 0.1f, true);
	}
}

void APlayerCharacter::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//When not overlapping.... stop timer and stop taking dmg
	GetWorld()->GetTimerManager().ClearTimer(DamageTimeHandle);
}

//When collision is made timer starts calling this function to deal dmg
void APlayerCharacter::DealDamage()
{
	playerHealth -= 1;

	if (playerHealth <= 0)
	{
		deathScreen();
		this->Destroy();
	}
}

void APlayerCharacter::shouldDisplayCrosshair(bool isADS)
{
	while (isADS) displayCrosshair();

	hideCrosshair();
}
