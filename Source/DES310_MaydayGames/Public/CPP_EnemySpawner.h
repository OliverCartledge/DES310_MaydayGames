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
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	FTimerHandle Timer;

	UPROPERTY(EditAnywhere, Category = "AI")
	UEnvQuery *RandomSpawnEQS;

	UFUNCTION()
	void RandomSpawn(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	UFUNCTION()
	void spawnEnemy();

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACPP_Enemy> enemyClass;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
