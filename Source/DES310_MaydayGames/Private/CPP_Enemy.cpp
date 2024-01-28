// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_Enemy.h"

// Sets default values
ACPP_Enemy::ACPP_Enemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	EnemyMesh =  CreateDefaultSubobject<USkeletalMeshComponent>("EnemyMesh");
	ArrowComponent = CreateDefaultSubobject<UArrowComponent>("ArrowComponent");
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>("CapsuleComponent");
	PawnSensing = CreateDefaultSubobject<FSeePawnDelegate>("PawnSensing");
	CharMoveComp = CreateDefaultSubobject<UCharacterMovementComponent>("CharMoveComp");
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	
	SetRootComponent(CapsuleCollision);
	EnemyMesh->SetupAttachment(CapsuleCollision);
	ArrowComponent->SetupAttachment(EnemyMesh);
	
	seeingPlayer = false;
	
	//TODO:
	//unsure how to add pawn sense  and  char move comp atm.
	
	

}

// Called when the game starts or when spawned
void ACPP_Enemy::BeginPlay()
{
	Super::BeginPlay();
	
	if (PawnSensingComp)
    {
        PawnSensingComp->OnSeePawn.AddDynamic(this, &ACPP_Enemy::OnSeePawn);
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

    if(seeingPlayer)
    {
    //TODO
    //playerFollow funciton
    }
}

void ACPP_Enemy::playerFollow()
{
    if(seeingPlayer)
    {
        
    }
}


