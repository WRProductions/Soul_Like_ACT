// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_LoginHud.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SoulGameInstanceBase.h"
#include "SoulSaveGame.h"


void UWidget_LoginHud::CacheSaveSlot()
{
	USoulGameInstanceBase* MyGI = Cast<USoulGameInstanceBase>(GetGameInstance());
	if (MyGI && MyGI->GetSaveSlot())
	{
		StartGame->SetIsEnabled(true);
		ResetButton->SetIsEnabled(true);
		PlayerID->SetVisibility(ESlateVisibility::Visible);
		PlayerID->SetText(FText::FromString(MyGI->GetSaveSlot()->UserId));
	}
	else
	{
		PlayerID->SetVisibility(ESlateVisibility::Hidden);
		PlayerID->SetIsEnabled(false);
		ResetButton->SetIsEnabled(true);
	}
}

void UWidget_LoginHud::BindButtons()
{
	USoulGameInstanceBase* MyGI = Cast<USoulGameInstanceBase>(GetGameInstance());
	if (MyGI)
	{
		StartGame->OnClicked.AddDynamic(MyGI, &USoulGameInstanceBase::OnStartGameClicked);

		ResetButton->OnClicked.AddDynamic(MyGI, &USoulGameInstanceBase::ResetSaveGame);
	}
}
