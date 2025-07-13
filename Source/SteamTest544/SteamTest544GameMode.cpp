// Copyright Epic Games, Inc. All Rights Reserved.

#include "SteamTest544GameMode.h"
#include "SteamTest544Character.h"
#include "UObject/ConstructorHelpers.h"

ASteamTest544GameMode::ASteamTest544GameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
