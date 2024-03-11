// Fill out your copyright notice in the Description page of Project Settings.
#include "CPP_Enemy.h"
#include "AIController.h"
#include "Engine.h"
#include "NavigationSystem.h" // Ensure this is included for navigation system access
#include "AI/Navigation/NavigationTypes.h" // For FNavLocation
#include "GameFramework/CharacterMovementComponent.h" // For character movement component
#include "DES310_MaydayGamesCharacter.generated.h"

// Sets default values
ACPP_Enemy::ACPP_Enemy()
{
    //set this character to call Tick() every frame.
    PrimaryActorTick.bCanEverTick = true;

    seeingPlayer = false;

    //set up and enable pawn sensing (using by AI to sense player)
    PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensing");
    PawnSensing->bEnableSensingUpdates = true;
    PawnSensing->SetPeripheralVisionAngle(180.f);

    Tags.Add(FName("Enemy"));
}

void ACPP_Enemy::BeginPlay()
{
    Super::BeginPlay();

    //add pawn sensing's "OnSeePawn" - this triggers when the AI 'sees' the player pawn
    if (PawnSensing)
    {
        PawnSensing->OnSeePawn.AddDynamic(this, &ACPP_Enemy::OnSeePawn); // Call OnSeePawn when a pawn is seen
    }
}

// Called every frame
void ACPP_Enemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!EnemyJumpTimer.IsValid() && seeingPlayer)
    {
        EnemyJumpTimer = FTimerHandle();
        GetWorldTimerManager().SetTimer(EnemyJumpTimer, this, &ACPP_Enemy::EnemyJump, 3.5f, false);
    }

    IsWithinNavMeshProxy();
}

// Called to bind functionality to input
void ACPP_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ACPP_Enemy::OnSeePawn(APawn* Pawn) 
{

    if (Pawn->ActorHasTag("Player"))
    {
        seeingPlayer = true;

        // If the player is seen
        if (seeingPlayer)
        {
            AAIController* AIController = GetController<AAIController>();

            if (AIController)
            {
                AIController->MoveToActor(Pawn);
            }
        }
    }
}

void ACPP_Enemy::EnemyJump()
{
    Jump();
    EnemyJumpTimer.Invalidate();
}

void ACPP_Enemy::EnemyJumpToLedge()
{
    //character component that handles movement
    UCharacterMovementComponent* LocalCharacterMovement = GetCharacterMovement();
    if (LocalCharacterMovement)
    {
        //upward impulse to simulate a jump
        FVector JumpForceLedge = FVector(0, 0, 55); // Example force, adjust as needed
        LocalCharacterMovement->AddImpulse(JumpForceLedge, true);
    }
}

void ACPP_Enemy::EnemyJumpToHighLedge()
{
    //character component that handles movement
    UCharacterMovementComponent* LocalCharacterMovement = GetCharacterMovement();
    if (LocalCharacterMovement)
    {
        //upward impulse to simulate a jump
        FVector JumpForceLedge = FVector(0, 0, 105); // Example force, adjust as needed
        LocalCharacterMovement->AddImpulse(JumpForceLedge, true);
    }
}

void ACPP_Enemy::EnemyJumpToReallyHighLedge()
{
    //character component that handles movement
    UCharacterMovementComponent* LocalCharacterMovement = GetCharacterMovement();
    if (LocalCharacterMovement)
    {
        //upward impulse to simulate a jump
        FVector JumpForceLedge = FVector(0, 0, 175); // Example force, adjust as needed
        LocalCharacterMovement->AddImpulse(JumpForceLedge, true);
    }
}

bool ACPP_Enemy::IsWithinNavMeshProxy()
{
    //get the AI's current location
    FVector AILocation = GetActorLocation();
    FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();


    //get NavMesh current location
    FNavLocation NavLocation;
    const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());

    //jump threasholds
    float LowThreashold = 150.f;
    float HighThreashold = 300.f;
    float ReallyHighThreashold = 500.f;

    //proximity checks
    float ProximityThreashold = 500.f;
    float DistanceToPlayer = FVector::Dist(AILocation, PlayerLocation);

    //if AI is near player and under the player, jump
    if(DistanceToPlayer <= ProximityThreashold)
    {
        if (NavSystem && NavSystem->ProjectPointToNavigation(AILocation, NavLocation) && AILocation.Z + LowThreashold < PlayerLocation.Z)
        {
            EnemyJumpToLedge();
        }
        else if (NavSystem && NavSystem->ProjectPointToNavigation(AILocation, NavLocation) && AILocation.Z + HighThreashold < PlayerLocation.Z)
        {
            EnemyJumpToHighLedge();
        }
        else if (NavSystem && NavSystem->ProjectPointToNavigation(AILocation, NavLocation) && AILocation.Z + ReallyHighThreashold < PlayerLocation.Z)
        {
            EnemyJumpToReallyHighLedge();
        }
    }

    return false;
}
