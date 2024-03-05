// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ExtractionZone.generated.h"

UCLASS()
class DES310_MAYDAYGAMES_API AExtractionZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExtractionZone();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Extraction")
	FBox ExtractionBox;

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Default")
	UBoxComponent* CollisionBox;

	UPROPERTY(EditAnywhere)
	bool HasExtracted;


	FTimerHandle ExtractionTimer;
	void ExtractionComplete();
	//void TimerExpired();
	//void StartTimer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool IsExtracting;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
