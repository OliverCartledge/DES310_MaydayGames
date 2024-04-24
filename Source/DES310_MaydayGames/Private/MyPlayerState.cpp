// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"

//Update grenade luncher ammo over time
void AMyPlayerState::IncreaseGrenade()
{
	currentGrenade += 1;
}

void AMyPlayerState::updateScore(int pointsToGive)
{
	const int32 oldScore = playerScore;
	//pointsToGive = FMath::Max(1, pointsToGive); 

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

void AMyPlayerState::updateGrenadeCount(int grenadeUpdate)
{

	int32 TempGrenadeCount = grenadeUpdate;

	currentGrenade = TempGrenadeCount;

	GrenadeCounter.Broadcast(currentGrenade);
}

void AMyPlayerState::updateObjCount(int objScoreUpdate)
{
	const int32 tempObjScore = objScoreUpdate;

	objScore += tempObjScore;

	ObjectiveCounter.Broadcast(objScore, tempObjScore);

	if (objScore >= 3)
	{
		//extraction becomes true
		bcanExtract = true;
	}
}


