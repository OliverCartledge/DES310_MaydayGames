// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerState.h"



void AMyPlayerState::updateScore(int pointsToGive)
{
	const int32 oldScore = playerScore;
	pointsToGive = FMath::Max(1, pointsToGive); 

	playerScore += pointsToGive;


	OnScoreUpdated.Broadcast(playerScore, oldScore);


	if (playerScore == 200) {}//call winscreen

}

