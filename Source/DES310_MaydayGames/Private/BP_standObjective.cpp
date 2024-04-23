// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_standObjective.h"
#include "Engine.h"
#include "MyPlayerState.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ABP_standObjective::ABP_standObjective()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	objectiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("objectiveMesh"));
	collisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("collisionSphere"));

	objectiveMesh->SetupAttachment(RootComponent);
	collisionSphere->SetupAttachment(objectiveMesh);

	collisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABP_standObjective::BeginOverlap);
	collisionSphere->OnComponentEndOverlap.AddDynamic(this, &ABP_standObjective::EndOverlap);

	Tags.Add(FName("Objective"));
	objComplete = false;
}

// Called when the game starts or when spawned
void ABP_standObjective::BeginPlay()
{
	Super::BeginPlay();

	//Initialize MyPlayerState to be used later when beginPlay is called
	MyPlayerState = Cast<AMyPlayerState>(UGameplayStatics::GetPlayerState(GetWorld()->GetFirstPlayerController(), 0));
}

// Called every frame
void ABP_standObjective::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABP_standObjective::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//Check if the overlapping actor is the player and only allow the player to start the objective
	if (OtherActor != nullptr && OtherActor->ActorHasTag("Player"))
	{
		//After X seconds, call the ObjectiveSucceed function
		GetWorld()->GetTimerManager().SetTimer(objectiveTimer, this, &ABP_standObjective::objectiveSucceed, 5.f, false);
	}
}

// Implement EndOverlap event
void ABP_standObjective::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!objComplete)
	{
		if (GetWorld())
		{
			if (OtherActor != nullptr && OtherActor->ActorHasTag("Player"))
			{
				//Reset the timer if the player walks off the objective pad
				GetWorld()->GetTimerManager().ClearTimer(objectiveTimer);
			}
		}
	}
}


void ABP_standObjective::objectiveSucceed()
{
	//Update both player score and objective score if the objective is completed successfully
	MyPlayerState->updateScore(scoreToGive);
	MyPlayerState->updateObjCount(objIncrement);

	objComplete = true;

	if (ObjectiveCompleteSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ObjectiveCompleteSound, GetActorLocation());
	}

	//Destroy the objective once complete to stop the player from repeatidly standing on the same objective pad
	Destroy();
	
}

