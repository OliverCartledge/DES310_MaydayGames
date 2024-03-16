// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/EngineTypes.h"

#include "AIController.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

#include "CPP_EnemySpawner.generated.h"

class ACPP_Enemy;
struct FTimerHandle;
struct FEnvQueryRequest;

UCLASS()
class DES310_MAYDAYGAMES_API ACPP_EnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPP_EnemySpawner();
	
	//timer - used ot spawn an enemy every 'x' seconds
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	FTimerHandle Timer;


	//query - used to give the AI's spawn logic
	UPROPERTY(EditAnywhere, Category = "AI")
	UEnvQuery *RandomSpawnEQS;

	UPROPERTY(EditAnywhere, Category = "AI")
		float spawnTimer = 2.7;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		TSubclassOf<ACPP_Enemy> enemyClass;

	//funcitons
	UFUNCTION()
	void RandomSpawn(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	UFUNCTION()
	void spawnEnemy();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
