// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "MyPlayerState.h"
#include "Kismet/GameplayStatics.h" 
#include "Materials/Material.h"

#include "BP_standObjective.generated.h"

class USTATICMESHCOMPONENT;
class USPHERECOMPONENT;
class UMATERIAL;

UCLASS()
class DES310_MAYDAYGAMES_API ABP_standObjective : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABP_standObjective();

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* objectiveMesh;

	UPROPERTY(EditAnywhere, category = "Collision")
	USphereComponent* collisionSphere;

	UFUNCTION(BlueprintCallable, category = "Objecive")
	void objectiveSucceed();

	//UPROPERTY(EditAnywhere, category = "Material")
	//UMaterial* startMaterial;

	//UPROPERTY(EditAnywhere, category = "Material")
	//UMaterial* endMaterial;
	


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FTimerHandle objectiveTimer;
	int32 scoreToGive = 100;
	int32 objIncrement = 1;


	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool objComplete;

private:
	AMyPlayerState* MyPlayerState;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
