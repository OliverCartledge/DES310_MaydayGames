// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerScoreUpdated, int32, newScore, int32, oldScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAmmoCounter, int32, newAmmoCount, int32, oldAmmoCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGrenadeCounter, int32, newGrenadeCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FObjectiveCounter, int32, newObjCount, int32, oldObjCount);

/**
 * 
 */
UCLASS()
class DES310_MAYDAYGAMES_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:


	//Getters 
	UFUNCTION(BlueprintGetter)
	float myGetScore() const
	{
		return playerScore;
	}

	UFUNCTION(BlueprintGetter)
	float myGetObj() const
	{
		return objScore;
	}

	UPROPERTY(BlueprintReadOnly, category = "obj")
		bool bcanExtract = false;

	//Update functions
	UFUNCTION(BlueprintCallable, category = "score")
		void updateScore(int pointsToGive);

	UFUNCTION(BlueprintCallable, category = "ammo")
		void updateAmmoCount(int ammoUpdate);

	UFUNCTION(BlueprintCallable, category = "ammo")
		void updateGrenadeCount(int grenadeUpdate);

	UFUNCTION(BlueprintCallable, category = "obj")
		void updateObjCount(int objScoreUpdate);

	UFUNCTION(BlueprintCallable, category = "obj")
		void IncreaseGrenade();

	

	//delegate inits
	UPROPERTY(BlueprintAssignable, category = "score")
		FOnPlayerScoreUpdated OnScoreUpdated;

	UPROPERTY(BlueprintAssignable, category = "ammo")
		FAmmoCounter AmmoCounter;

	UPROPERTY(BlueprintAssignable, category = "ammo")
		FGrenadeCounter GrenadeCounter;

	UPROPERTY(BlueprintAssignable, category = "obj")
		FObjectiveCounter ObjectiveCounter;




protected: 
	UPROPERTY(BlueprintReadOnly, category = "score")
		int32 playerScore;

	UPROPERTY(BlueprintReadOnly, category = "score")
		int32 objScore = 0;

	UPROPERTY(BlueprintReadOnly, category = "ammo")
		int32 currentAmmo = 40;

	UPROPERTY(BlueprintReadOnly, category = "ammo")
		int32 maxAmmo = 40;

	UPROPERTY(BlueprintReadOnly, category = "ammo")
		int32 currentGrenade = 3;

	UPROPERTY(BlueprintReadOnly, category = "ammo")
		int32 maxGrenade = 3;


};
