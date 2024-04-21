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

    PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensing");
    PawnSensing->bEnableSensingUpdates = true;
    PawnSensing->SetPeripheralVisionAngle(180.f);

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent);

    EnemyJumpTimer = FTimerHandle();


    Tags.Add(FName("Enemy"));
}

void ACPP_Enemy::BeginPlay()
{
    Super::BeginPlay();

    if (IsValid(PawnSensing))
    {
        PawnSensing->OnSeePawn.AddDynamic(this, &ACPP_Enemy::OnSeePawn); 
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
    UCharacterMovementComponent* LocalCharacterMovement = GetCharacterMovement();
    if (LocalCharacterMovement)
    {
        FVector JumpForceLedge = FVector(0, 0, 60); 
        LocalCharacterMovement->AddImpulse(JumpForceLedge, true);
    }
}

void ACPP_Enemy::EnemyJumpToHighLedge()
{
    UCharacterMovementComponent* LocalCharacterMovement = GetCharacterMovement();
    if (LocalCharacterMovement)
    {
  
        FVector JumpForceLedge = FVector(0, 0, 105); 
        LocalCharacterMovement->AddImpulse(JumpForceLedge, true);
    }
}

void ACPP_Enemy::EnemyJumpToReallyHighLedge()
{

    UCharacterMovementComponent* LocalCharacterMovement = GetCharacterMovement();
    if (LocalCharacterMovement)
    {
        
        FVector JumpForceLedge = FVector(0, 0, 170); 
        LocalCharacterMovement->AddImpulse(JumpForceLedge, true);
    }
}

void ACPP_Enemy::IsWithinNavMeshProxy()
{
    if (!IsValid(GetWorld()->GetFirstPlayerController()->GetPawn()))
    {
        return;
    }

    //log pos's
    FVector AILocation = GetActorLocation();
    FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();

    //threasholds
    float TeleportThreshold = 5600.f;
    float DistanceToPlayer = FVector::Dist(AILocation, PlayerLocation);
    float ApproachThreshold = 1.f;
    float jumpThreashold = 800.f;

    ////teleport
    if (DistanceToPlayer >= TeleportThreshold)
    {
        FVector NewLocation = PlayerLocation + (AILocation - PlayerLocation).GetSafeNormal() * TeleportThreshold;

        SetActorLocation(NewLocation);

        AAIController* AIController = GetController<AAIController>();
        if (IsValid(AIController))
        {
            AIController->MoveToActor(GetWorld()->GetFirstPlayerController()->GetPawn(), 1.f); 
        }
    }
    //approach
    else if (DistanceToPlayer > ApproachThreshold)
    {
        AAIController* AIController = GetController<AAIController>();
        if (IsValid(AIController))
        {
            AIController->MoveToActor(GetWorld()->GetFirstPlayerController()->GetPawn(), 1.f); 
        }
    }
    //jump
    if(DistanceToPlayer <= jumpThreashold)
    {
        float LowThreashold = 150.f;
        float HighThreashold = 300.f;
        float ReallyHighThreashold = 500.f;

        float ProximityThreashold = 800.f;

        if (DistanceToPlayer <= ProximityThreashold)
        {
            if (AILocation.Z + LowThreashold < PlayerLocation.Z)
            {
                EnemyJumpToLedge();
            }
            else if (AILocation.Z + HighThreashold < PlayerLocation.Z)
            {
                EnemyJumpToHighLedge();
            }
            else if (AILocation.Z + ReallyHighThreashold < PlayerLocation.Z)
            {
                EnemyJumpToReallyHighLedge();
            }
        }
    }
}
