// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"


#include "BP_standObjective.generated.h"

class USTATICMESHCOMPONENT;
class USPHERECOMPONENT;
//class USKELETALCMESHCOMPONENT;

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


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
