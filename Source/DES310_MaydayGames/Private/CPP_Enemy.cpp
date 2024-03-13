#include "CPP_Enemy.h"
#include "AIController.h"
#include "Engine.h"
#include "NavigationSystem.h" 
#include "AI/Navigation/NavigationTypes.h" 
#include "GameFramework/CharacterMovementComponent.h" 
#include "DES310_MaydayGamesCharacter.generated.h"

ACPP_Enemy::ACPP_Enemy()
{
    PrimaryActorTick.bCanEverTick = true;

    seeingPlayer = false;

    //set up and enable pawn sensing (using by AI to sense player)
    PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensing");
    PawnSensing->bEnableSensingUpdates = true;
    PawnSensing->SetPeripheralVisionAngle(180.f);

    EnemyJumpTimer = FTimerHandle();

    Tags.Add(FName("Enemy"));
}

void ACPP_Enemy::BeginPlay()
{
    Super::BeginPlay();

    //add pawn sensing's "OnSeePawn" - this triggers when the AI 'sees' the player pawn
    if (IsValid(PawnSensing))
    {
        PawnSensing->OnSeePawn.AddDynamic(this, &ACPP_Enemy::OnSeePawn); // Call OnSeePawn when a pawn is seen
    }

    if (CheckNavMeshTimerHandle.IsValid())
    {
        GetWorldTimerManager().SetTimer(CheckNavMeshTimerHandle, this, &ACPP_Enemy::IsWithinNavMeshProxy, 1.0f, true);
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
    if (IsValid(Pawn) && Pawn->ActorHasTag("Player"))
    {
        seeingPlayer = true;

        // If the player is seen
        if (seeingPlayer)
        {
            AAIController* AIController = GetController<AAIController>();

            if (IsValid(AIController))
            {
                AIController->MoveToActor(Pawn);
            }
        }
    }
}

void ACPP_Enemy::EnemyJump()
{
    Jump();
    if (EnemyJumpTimer.IsValid())
    {
        EnemyJumpTimer.Invalidate();
    }
}

void ACPP_Enemy::EnemyJumpToLedge()
{
    //character component that handles movement
    UCharacterMovementComponent* LocalCharacterMovement = GetCharacterMovement();
    if (LocalCharacterMovement)
    {
        //upward impulse to simulate a jump
        FVector JumpForceLedge = FVector(0, 0, 60); // Example force, adjust as needed
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
        FVector JumpForceLedge = FVector(0, 0, 170); // Example force, adjust as needed
        LocalCharacterMovement->AddImpulse(JumpForceLedge, true);
    }
}

void ACPP_Enemy::IsWithinNavMeshProxy()
{

    //check for player
    if (!IsValid(GetWorld()->GetFirstPlayerController()->GetPawn()))
    {
        return;
    }

    //get the AI's current location
    FVector AILocation = GetActorLocation();
    FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

    //jump threasholds
    float LowThreashold = 150.f;
    float HighThreashold = 300.f;
    float ReallyHighThreashold = 500.f;

    //proximity checks
    float ProximityThreashold = 650.f;
    float DistanceToPlayer = FVector::Dist(AILocation, PlayerLocation);

    //if AI is near player and under the player, jump
    if (DistanceToPlayer <= ProximityThreashold)
    {
        if (AILocation.Z + LowThreashold < PlayerLocation.Z)
        {
            EnemyJumpToLedge();
            //GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Jump activated")));
        }
        else if (AILocation.Z + HighThreashold < PlayerLocation.Z) 
        {
            EnemyJumpToHighLedge();
            //GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("High Jump activated")));
        }
        else if (AILocation.Z + ReallyHighThreashold < PlayerLocation.Z)
        {
            EnemyJumpToReallyHighLedge();
            //GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Super High Jump activated")));
        }
    }
}

