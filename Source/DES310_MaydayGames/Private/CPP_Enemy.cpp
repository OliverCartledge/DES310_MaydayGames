// Fill out your copyright notice in the Description page of Project Settings.
#include "CPP_Enemy.h"
#include "AIController.h"
#include "DES310_MaydayGamesCharacter.generated.h"


// Sets default values
ACPP_Enemy::ACPP_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	seeingPlayer = false;
	
	//set up and enable pawn sensing (using by AI to sense player)
	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensing");
	PawnSensing->bEnableSensingUpdates = true;
	PawnSensing->SetPeripheralVisionAngle(180.f);

	Tags.Add(FName("Enemy"));
}

// Called when the game starts or when spawned
void ACPP_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
	//add pawn sensing's "OnSeePawn" - this triggers when the AI 'see's' the player pawn
	if (PawnSensing)
    {
		PawnSensing->OnSeePawn.AddDynamic(this, &ACPP_Enemy::OnSeePawn); //call OnSeePawn when a pawn is seen
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


//OnSeePawn function (created)
//This is what enables the AI to 'see' the player pawn
void ACPP_Enemy::OnSeePawn(APawn* Pawn) //player or 'other' pawn passed in via param
{

	//check for player tag to allow this ot see the player, specifically
	if(Pawn->ActorHasTag("Player"))
	{
		seeingPlayer = true;

		//if the player is see
		if(seeingPlayer)
		{
			//AI MOVETO:
				//get the AI's controller to override/update it
			AAIController* AIController = GetController<AAIController>();

			//move the AI to the player (pawn actor)
			if (AIControllerClass != nullptr)
			{
				AIController->MoveToActor(Pawn);
			}
		}
	}
}




