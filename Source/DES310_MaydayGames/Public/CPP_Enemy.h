// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/StaticMeshComponent.h"
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
	
	UFUNCTION(BlueprintCallable)
    void playerFollow()
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	bool seeingPlayer;
	
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* EnemyMesh;
	
	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleCollision;
	
	UPROPERTY(VisibleAnywhere)
	UArrowComponent* ArrrowComp;
	
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;
	
	UPROPERTY(VisibleAnywhere)
	UCharacterMovementComponent* CharMoveComp;
	
    UPROPERTY(VisibleAnywhere)
    UPawnSensingComponent* PawnSensingComp;


	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};