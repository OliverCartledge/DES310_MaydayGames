// Fill out your copyright notice in the Description page of Project Settings.
#include "CPP_Enemy.h"
#include "AIController.h"


// Sets default values
ACPP_Enemy::ACPP_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	seeingPlayer = false;
	
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensing");
	PawnSensing->bEnableSensingUpdates = true;
	PawnSensing->SetPeripheralVisionAngle(180.f);
}

// Called when the game starts or when spawned
void ACPP_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (PawnSensing)
    {
        PawnSensing->OnSeePawn.AddDynamic(this, &ACPP_Enemy::OnSeePawn);
    }
	
}

// Called every frame
void ACPP_Enemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACPP_Enemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACPP_Enemy::OnSeePawn(APawn* Pawn)
{
	if(Pawn->ActorHasTag("Player"))
	{
		seeingPlayer = true;
		if(seeingPlayer)
		{
			//AI MOVETO
			AAIController* AIController = GetController<AAIController>();

			if (AIControllerClass != nullptr)
			{
				AIController->MoveToActor(Pawn);
			}
		}
	}
}



