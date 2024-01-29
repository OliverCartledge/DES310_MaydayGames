// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "Components/SkeletalMeshComponent.h"
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
	
	UFUNCTION()
	void OnSeePawn(APawn* Pawn);
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	bool seeingPlayer;
	
	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* PawnSensing;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
