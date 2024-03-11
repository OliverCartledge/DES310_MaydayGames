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
    // Set this character to call Tick() every frame.
    PrimaryActorTick.bCanEverTick = true;

    seeingPlayer = false;

    // Set up and enable pawn sensing (using by AI to sense player)
    PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensing");
    PawnSensing->bEnableSensingUpdates = true;
    PawnSensing->SetPeripheralVisionAngle(180.f);

    Tags.Add(FName("Enemy"));
}

// Called when the game starts or when spawned
void ACPP_Enemy::BeginPlay()
{
    Super::BeginPlay();

    // Add pawn sensing's "OnSeePawn" - this triggers when the AI 'sees' the player pawn
    if (PawnSensing)
    {
        PawnSensing->OnSeePawn.AddDynamic(this, &ACPP_Enemy::OnSeePawn); // Call OnSeePawn when a pawn is seen
    }
}

// Called every frame
void ACPP_Enemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

  /*  if (!EnemyJumpTimer.IsValid() && seeingPlayer)
    {
        EnemyJumpTimer = FTimerHandle();
        GetWorldTimerManager().SetTimer(EnemyJumpTimer, this, &ACPP_Enemy::EnemyJump, 1.0f, false);
    }*/


    //worst case:
        //if enemy colides with proxy
            //apply upwards force and impulse
                //eg: 
                        //    FVector JumpForce = FVector(0, 0, 10); // Adjust force as needed
                        //    LocalCharacterMovement->AddImpulse(JumpForce, true);


    IsWithinNavMeshProxy();

    /*if (IsWithinNavMeshProxy())
    {
        EnemyJump();
    }*/
  /*  else if (!EnemyJumpTimer.IsValid() && seeingPlayer)
    {
        EnemyJumpTimer = FTimerHandle();
        GetWorldTimerManager().SetTimer(EnemyJumpTimer, this, &ACPP_Enemy::EnemyJump, 1.0f, false);
    }*/
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
    GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy jump"));  //debug
    Jump();
    EnemyJumpTimer.Invalidate();
}

void ACPP_Enemy::EnemyJumpToLedge()
{
    // Character component that handles movement
    UCharacterMovementComponent* LocalCharacterMovement = GetCharacterMovement();
    if (LocalCharacterMovement)
    {
        // Upward impulse to simulate a jump
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy jump"));  //debug
        FVector JumpForceLedge = FVector(0, 0, 45); // Example force, adjust as needed
        LocalCharacterMovement->AddImpulse(JumpForceLedge, true);
    }
}

void ACPP_Enemy::EnemyJumpToHighLedge()
{
    // Character component that handles movement
    UCharacterMovementComponent* LocalCharacterMovement = GetCharacterMovement();
    if (LocalCharacterMovement)
    {
        // Upward impulse to simulate a jump
        GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy jump"));  //debug
        FVector JumpForceLedge = FVector(0, 0, 80); // Example force, adjust as needed
        LocalCharacterMovement->AddImpulse(JumpForceLedge, true);
    }
}

bool ACPP_Enemy::IsWithinNavMeshProxy()
{
    // Get the AI's current location
    FVector AILocation = GetActorLocation();
    FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();


    // Get NavMesh current location
    FNavLocation NavLocation;
    const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
    float LowThreashold = 100;
    float HighThreashold = 250;

    if (NavSystem && NavSystem->ProjectPointToNavigation(AILocation, NavLocation) && AILocation.Z + LowThreashold < PlayerLocation.Z)
    {
        //add: is player not jump
        //or, most likey better, code a funciton which uses a threashold to stop the bouncing
        EnemyJumpToLedge();
    }
    else if (NavSystem && NavSystem->ProjectPointToNavigation(AILocation, NavLocation) && AILocation.Z + HighThreashold < PlayerLocation.Z)
    {
        //add: is player not jump
        //or, most likey better, code a funciton which uses a threashold to stop the bouncing
        EnemyJumpToHighLedge();
    }

    // The AI is not within the NavMesh
    UE_LOG(LogTemp, Warning, TEXT("AI is not within NavMesh proxy."));
    return false;
}
