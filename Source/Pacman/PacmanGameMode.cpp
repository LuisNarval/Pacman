// Copyright Epic Games, Inc. All Rights Reserved.

#include "PacmanGameMode.h"
#include "PacmanCharacter.h"
#include "UObject/ConstructorHelpers.h"

APacmanGameMode::APacmanGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
