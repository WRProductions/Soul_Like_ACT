// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulGameInstanceBase.h"
#include "SoulSaveGame.h"
#include "Item/ItemBasic.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "SoulAssetManager.h"



USoulGameInstanceBase::USoulGameInstanceBase()
	: SaveSlot(TEXT("SaveGame"))
	, SaveUserIndex(0)
{}

void USoulGameInstanceBase::GetAllAccessibleItemID(TArray<FPrimaryAssetId>& OutpId)
{
	//Clear the assets id if we already called this before
	OutpId.Reset();

	for (auto AssetType : AllItemTypes)
	{
		GetItemIDWithType(AssetType, OutpId);
	}
}

void USoulGameInstanceBase::GetItemIDWithType(const FPrimaryAssetType ItemType, TArray<FPrimaryAssetId>& OutpId)
{
	USoulAssetManager& CurrentAssetManager = USoulAssetManager::Get();

	CurrentAssetManager.GetPrimaryAssetIdList(ItemType, OutpId);
}

void USoulGameInstanceBase::AddDefaultInventory(USoulSaveGame* SaveGame, bool bRemoveExtra)
{
	// If we want to remove extra, clear out the existing inventokay
 	if (bRemoveExtra)
 	{
 		//SaveGame->InventoryData.Reset();
 	}

 	
}

bool USoulGameInstanceBase::IsValidItemSlot(FSoulItemSlot ItemSlot) const
{
	return false;
}

bool USoulGameInstanceBase::LoadOrCreateSaveGame()
{
	// Drop reference to old save game, this will GC out
	CurrentSaveGame = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, SaveUserIndex) && bSavingEnabled)
	{
		CurrentSaveGame = Cast<USoulSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, SaveUserIndex));
	}

	if (CurrentSaveGame)
	{
		// Make sure it has any newly added default inventory
		AddDefaultInventory(CurrentSaveGame, false);

		return true;
	}
	else
	{
		// This creates it on demand
		CurrentSaveGame = Cast<USoulSaveGame>(UGameplayStatics::CreateSaveGameObject(USoulSaveGame::StaticClass()));

		AddDefaultInventory(CurrentSaveGame, true);

		return false;
	}
}

bool USoulGameInstanceBase::WriteSaveGame()
{
	if (bSavingEnabled)
	{
		return UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlot, SaveUserIndex);
	}
	return false;
}

void USoulGameInstanceBase::ResetSaveGame()
{
	bool bWasSavingEnabled = bSavingEnabled;
	bSavingEnabled = false;
	LoadOrCreateSaveGame();
	bSavingEnabled = bWasSavingEnabled;
}
