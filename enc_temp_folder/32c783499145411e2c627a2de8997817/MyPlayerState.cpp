// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"



void AMyPlayerState::updateScore(int pointsToGive)
{
	const int32 oldScore = playerScore;
	pointsToGive = FMath::Max(1, pointsToGive); 

	playerScore += pointsToGive;


	OnScoreUpdated.Broadcast(playerScore, oldScore);


	if (playerScore == 200) {}//create & call winscreen

}

void AMyPlayerState::updateAmmoCount(int ammoUpdate)
{

	int32 TempAmmoCount = ammoUpdate;

	currentAmmo = TempAmmoCount;

	AmmoCounter.Broadcast(currentAmmo, TempAmmoCount);
}

