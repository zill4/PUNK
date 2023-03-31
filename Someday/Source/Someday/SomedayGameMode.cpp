// Copyright Epic Games, Inc. All Rights Reserved.

#include "SomedayGameMode.h"
#include "SomedayCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASomedayGameMode::ASomedayGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
