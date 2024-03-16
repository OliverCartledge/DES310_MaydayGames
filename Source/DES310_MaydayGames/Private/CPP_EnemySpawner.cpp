// Fill out your copyright notice in the Description page of Project Settings.


#include "CPP_EnemySpawner.h"
#include "Engine/EngineTypes.h"
#include "AIController.h"
#include "CPP_Enemy.h"
#include "EnvironmentQuery/EnvQueryManager.h"


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

	//start the timer when the game starts
		//this sets the timer, allowing 'this' class to use the spawnEnemy funciton every 1.5 seconds. Looping is enabled.
	GetWorld()->GetTimerManager().SetTimer(Timer, this, &ACPP_EnemySpawner::spawnEnemy, 5.f, true);
	
}

// Called every frame
void ACPP_EnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//get random spawn points, as a result of the query
void ACPP_EnemySpawner::RandomSpawn(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	//array to hold all spawn points from query
	TArray<FVector> spawnPositions = QueryInstance->GetResultsAsLocations();
	
	//as long as there are points to use...
	if(spawnPositions.Num() > 0)
	{
		//...spawn an actor from the enemy class at the stored position. Spawns in 'not' rotating.
		GetWorld()->SpawnActor<AActor>(enemyClass, spawnPositions[0], FRotator::ZeroRotator);
	}
}

//funciton to spawn enemy actors
void ACPP_EnemySpawner::spawnEnemy()
{
	//create query - used to 'run' the query using 'this class',
		//to get and store spawn points. See parameters.
	UEnvQueryInstanceBlueprintWrapper* query = UEnvQueryManager::RunEQSQuery(this, RandomSpawnEQS, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);

	//if a query exists...
	if(query != nullptr)
	{
		//...spawn the enemy at the random spawns, using 'this' class.
		query->GetOnQueryFinishedEvent().AddDynamic(this, &ACPP_EnemySpawner::RandomSpawn);
	}
}



