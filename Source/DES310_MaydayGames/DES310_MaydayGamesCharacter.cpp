// Copyright Epic Games, Inc. All Rights Reserved.

#include "DES310_MaydayGamesCharacter.h"

#include "Engine.h"
#include "CPP_Enemy.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


//////////////////////////////////////////////////////////////////////////
// ADES310_MaydayGamesCharacter

ADES310_MaydayGamesCharacter::ADES310_MaydayGamesCharacter()
{
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

	Tags.Add(FName("Player"));

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	//CollisionBox->SetBoxExtent();

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ADES310_MaydayGamesCharacter::BeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ADES310_MaydayGamesCharacter::EndOverlap);
}

void ADES310_MaydayGamesCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	playerHealth = 50;

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void ADES310_MaydayGamesCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADES310_MaydayGamesCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADES310_MaydayGamesCharacter::Look);

		//Input mapping for holding/ releasing right click
		EnhancedInputComponent->BindAction(ADSAction, ETriggerEvent::Started, this, &ADES310_MaydayGamesCharacter::ADSPressed);
		EnhancedInputComponent->BindAction(ADSAction, ETriggerEvent::Completed, this, &ADES310_MaydayGamesCharacter::ADSReleased);
	}
}


void ADES310_MaydayGamesCharacter::ADSPressed()
{
	// hmm this condition does work? 
	if (GetCharacterMovement()->IsFalling())
		InAirTest = true;

	if (!GetCharacterMovement()->IsFalling()) { //Isn't Jumping }
		IsADS = true;
		Crouch();
		
	}
}

void ADES310_MaydayGamesCharacter::ADSReleased()
{
	InAirTest = false;
	IsADS = false;
	UnCrouch();
}

void ADES310_MaydayGamesCharacter::Move(const FInputActionValue& Value)
{
	if (!IsADS && !InAirTest)
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

void ADES310_MaydayGamesCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADES310_MaydayGamesCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool ADES310_MaydayGamesCharacter::GetHasRifle()
{
	return bHasRifle;
}

bool ADES310_MaydayGamesCharacter::GetJumpStatus()
{
	return GetCharacterMovement()->IsFalling();
}

void ADES310_MaydayGamesCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor->IsA(ACPP_Enemy::StaticClass()))
	{
		GetWorld()->GetTimerManager().ClearTimer(HealingTimeHandle);
		//Damage player using a timer similar to how the full auto / fire rate is set up. deadass took me 2 hours to get this working :)
		GetWorld()->GetTimerManager().SetTimer(DamageTimeHandle, this, &ADES310_MaydayGamesCharacter::DealDamage, 0.1f, true);
	}
}

void ADES310_MaydayGamesCharacter::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//When not overlapping.... stop timer and stop taking dmg
	GetWorld()->GetTimerManager().ClearTimer(DamageTimeHandle);

	if (playerHealth >=1 && playerHealth <= 50)
	{
		GetWorld()->GetTimerManager().SetTimer(HealingTimeHandle, this, &ADES310_MaydayGamesCharacter::HealPlayer, 1.f, true);
	}
}

//When collision is made timer starts calling this function to deal dmg
void ADES310_MaydayGamesCharacter::DealDamage()
{
	playerHealth -= 1;

	if (playerHealth <= 0)
	{
		deathScreen();
		this->Destroy();
	}
}

//Heal the player after a set amount of time
void ADES310_MaydayGamesCharacter::HealPlayer()
{
	playerHealth += 1;
}

void ADES310_MaydayGamesCharacter::shouldDisplayCrosshair(bool isADS)
{
	while (isADS) displayCrosshair();

	hideCrosshair();
}
