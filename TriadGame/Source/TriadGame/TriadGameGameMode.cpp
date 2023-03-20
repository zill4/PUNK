// Copyright Epic Games, Inc. All Rights Reserved.

#include "TriadGameGameMode.h"
#include "TriadGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATriadGameGameMode::ATriadGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
