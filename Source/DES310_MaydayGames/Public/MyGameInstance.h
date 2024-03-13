// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CPP_Enemy.h"
//weapon component will be forward declared via 'class'
#include "MyGameInstance.generated.h"
//
//class TP_WeaponComponent;
//class DES310MaydayGamesCharacter;
 

/**
 * 
 */
UCLASS()
class DES310_MAYDAYGAMES_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:

	//UPROPERTY(BlueprintReadWrite, category = "score")
	//	int playerScore;

	//UFUNCTION(BlueprintReadWrite, category = "score")
	//	void updateScore();

	//ACPP_Enemy* myEnemy;
	//TP_WeaponComponent* myWeapon;
	//DES310MaydayGamesCharacter* myCharacter;
};
