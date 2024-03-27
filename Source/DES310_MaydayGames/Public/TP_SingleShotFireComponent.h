// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "MyPlayerState.h"
#include "TP_SingleShotFireComponent.generated.h"

class USphereComponent;

UCLASS(config = Game)
class DES310_MAYDAYGAMES_API ATP_SingleShotFireComponent : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* SphereComp;

	UPROPERTY(EditAnywhere, Category = "Mesh")
	UStaticMeshComponent* ExplosionMesh;

public:
	// Sets default values for this actor's properties
	ATP_SingleShotFireComponent();	
	
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Timer for full auto
	FTimerHandle ExplosionTimer;
	float grenadeDamage = 150;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TimerStart();

	void Explosion();

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return SphereComp; }

};
