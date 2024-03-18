// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_standObjective.h"

#include "MyPlayerState.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ABP_standObjective::ABP_standObjective()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//startMaterial = CreateDefaultSubobject<UMaterial>(TEXT("startMaterial"));
	//endMaterial = CreateDefaultSubobject<UMaterial>(TEXT("endMaterial"));

	objectiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("objectiveMesh"));
	collisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("collisionSphere"));

	objectiveMesh->SetupAttachment(RootComponent);
	collisionSphere->SetupAttachment(objectiveMesh);

	collisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ABP_standObjective::BeginOverlap);
	collisionSphere->OnComponentEndOverlap.AddDynamic(this, &ABP_standObjective::EndOverlap);

	Tags.Add(FName("Objective"));
	objComplete = false;
	//objectiveMesh->SetMaterial(0, startMaterial);
}

// Called when the game starts or when spawned
void ABP_standObjective::BeginPlay()
{
	Super::BeginPlay();

	//MyPlayerState = Cast<AMyPlayerState>(UGameplayStatics::GetPlayerState(GetWorld()->GetFirstPlayerController(), 0));
	MyPlayerState = Cast<AMyPlayerState>(UGameplayStatics::GetPlayerState(GetWorld()->GetFirstPlayerController(), 0));
}

// Called every frame
void ABP_standObjective::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABP_standObjective::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != nullptr && OtherActor->ActorHasTag("Player"))
	{
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
			GetWorld()->GetTimerManager().ClearTimer(objectiveTimer);
		}
	}
}


void ABP_standObjective::objectiveSucceed()
{
	MyPlayerState->updateScore(scoreToGive);
	MyPlayerState->updateObjCount(objIncrement);
	//objectiveMesh->SetMaterial(0, endMaterial);

	objComplete = true;
	
}

