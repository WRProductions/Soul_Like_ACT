// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Soul_Like_ACTGameMode.h"
#include "Soul_Like_ACTCharacter.h"
#include "SoulPlayerController.h"
#include "SLA_GameState.h"
#include "UObject/ConstructorHelpers.h"

ASoul_Like_ACTGameMode::ASoul_Like_ACTGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<ASoul_Like_ACTCharacter> PlayerPawnBPClass(TEXT("/Game/Player/Player_Character_BP"));
	static ConstructorHelpers::FClassFinder<ASoulPlayerController> PlayerControllerBPClass(TEXT("/Game/Player/SoulPlayerController_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	GameStateClass = ASLA_GameState::StaticClass();

}
