// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Soul_Like_ACTGameMode.h"
#include "Soul_Like_ACTCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASoul_Like_ACTGameMode::ASoul_Like_ACTGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<ATargetableActor> PlayerPawnBPClass(TEXT("/Game/Player/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
