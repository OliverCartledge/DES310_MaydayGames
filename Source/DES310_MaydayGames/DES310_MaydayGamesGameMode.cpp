// Copyright Epic Games, Inc. All Rights Reserved.

#include "DES310_MaydayGamesGameMode.h"
#include "DES310_MaydayGamesCharacter.h"
#include "UObject/ConstructorHelpers.h"

ADES310_MaydayGamesGameMode::ADES310_MaydayGamesGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
