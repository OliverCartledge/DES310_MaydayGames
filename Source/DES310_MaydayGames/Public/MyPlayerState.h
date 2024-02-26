// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerScoreUpdated, int32, newScore, int32, oldScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAmmoCounter, int32, newAmmoCount, int32, oldAmmoCount);

/**
 * 
 */
UCLASS()
class DES310_MAYDAYGAMES_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, category = "score")
		void updateScore(int pointsToGive);

	UFUNCTION(BlueprintCallable, category = "ammo")
		void updateAmmoCount(int ammoUpdate);



	UPROPERTY(BlueprintAssignable, category = "score")
		FOnPlayerScoreUpdated OnScoreUpdated;

	UPROPERTY(BlueprintAssignable, category = "ammo")
		FAmmoCounter AmmoCounter;






protected: 
	UPROPERTY(BlueprintReadOnly, category = "score")
		int32 playerScore;


	UPROPERTY(BlueprintReadOnly, category = "ammo")
		int32 currentAmmo = 40;




	//TODO:
	//winscreen
	//win conditions
	//game timer
	//move currentHealth from weapon to enemy and cast it over


};
