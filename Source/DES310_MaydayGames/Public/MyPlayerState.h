// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerScoreUpdated, int32, newScore, int32, oldScore);

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

	UPROPERTY(BlueprintAssignable, category = "score")
		FOnPlayerScoreUpdated OnScoreUpdated;




protected: 
	UPROPERTY(BlueprintReadOnly, category = "score")
		int playerScore;




	//TODO:
	//winscreen
	//win conditions
	//game timer
	//move currentHealth from weapon to enemy and cast it over


};
