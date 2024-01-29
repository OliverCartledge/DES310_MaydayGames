// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_EnemySpawner.h"
#include "Engine/EngineTypes.h"
#include "AIController.h"
#include "CPP_Enemy.h"
#include "EnvironmentQuery/EnvQueryManager.h"


//TODO
//Make simple spawner first, then add EQS.
//EQS is proving to be quite the task.




// Sets default values
ACPP_EnemySpawner::ACPP_EnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACPP_EnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ACPP_EnemySpawner::spawnEnemy, 1.5f, true);
	
}

// Called every frame
void ACPP_EnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPP_EnemySpawner::RandomSpawn(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	TArray<FVector> spawnPositions = QueryInstance->GetResultsAsLocations();
	
	if(spawnPositions.Num() > 0)
	{
		GetWorld()->SpawnActor<AActor>(enemyClass, spawnPositions[0], FRotator::ZeroRotator);
	}
}

void ACPP_EnemySpawner::spawnEnemy()
{
	UEnvQueryInstanceBlueprintWrapper* query = UEnvQueryManager::RunEQSQuery(this, RandomSpawnEQS, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if(query != nullptr)
	{
		query->GetOnQueryFinishedEvent().AddDynamic(this, &ACPP_EnemySpawner::RandomSpawn);
	}
}



