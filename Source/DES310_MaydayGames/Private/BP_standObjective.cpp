// Fill out your copyright notice in the Description page of Project Settings.


#include "BP_standObjective.h"

// Sets default values
ABP_standObjective::ABP_standObjective()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	objectiveMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("objectiveMesh"));
	collisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("collisionSphere"));

	collisionSphere->SetupAttachment(RootComponent); 
	objectiveMesh->SetupAttachment(RootComponent);

	Tags.Add(FName("Objective"));
}

// Called when the game starts or when spawned
void ABP_standObjective::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABP_standObjective::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

