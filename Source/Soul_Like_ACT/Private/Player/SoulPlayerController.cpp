// Fill out your copyright notice in the Description page of Project Settings.

#include "SoulPlayerController.h"
#include "Player/SoulUIManager.h"

ASoulPlayerController::ASoulPlayerController()
{
	UIManager = CreateDefaultSubobject<USoulUIManager>("UIManager");
}

USoulUIManager* ASoulPlayerController::GetUIManagerFromController(AController* InController)
{
	ASoulPlayerController* SoulController = Cast<ASoulPlayerController>(InController);

	if (!SoulController)
	{
		return nullptr;
	}
	return SoulController->GetUIManager();
}
