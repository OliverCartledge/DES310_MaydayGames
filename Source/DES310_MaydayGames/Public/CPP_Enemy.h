// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "CPP_Enemy.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UArrowComponent;
class UPawnSensingComponent;
class UCharacterMovementComponent;


UCLASS()
class DES310_MAYDAYGAMES_API ACPP_Enemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACPP_Enemy();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "health")
	float enemyHealth;

	UPROPERTY(EditAnywhere, category = "Score")
	int32 enemyGiveScore = 10;

	int32 objWinIndex = 0;
	
	//definition on 'pawn see' funciton
	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION()
	void EnemyJumpToLedge();

	UFUNCTION()
	void EnemyJumpToHighLedge();

	UFUNCTION()
	void EnemyJumpToReallyHighLedge();

	UFUNCTION()
	void EnemyJump();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void IsWithinNavMeshProxy();


	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	bool seeingPlayer;
	
	//used to define pawn sensing
	UPROPERTY(EditAnywhere)
	UPawnSensingComponent* PawnSensing;

	FTimerHandle CheckNavMeshTimerHandle;
	FTimerHandle EnemyJumpTimer;

	bool FShouldTeleport = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
