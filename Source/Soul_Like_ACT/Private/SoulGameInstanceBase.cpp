// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulGameInstanceBase.h"
#include "SoulSaveGame.h"
#include "Item/ItemBasic.h"
#include "Kismet/KismetSystemLibrary.h"
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
	// If we want to remove extra, clear out the existing inventory
 	if (bRemoveExtra)
 	{
 		SaveGame->InventoryData.Reset();
 	} 
 	// Now add the default inventory, this only adds if not already in hte inventory
 	for (const TPair<FPrimaryAssetId, FSoulItemData>& Pair : DefaultInventory)
 	{
 		if (!SaveGame->InventoryData.Contains(Pair.Key))
 		{
 			SaveGame->InventoryData.Add(Pair.Key, Pair.Value);
 		}
 	}
}

bool USoulGameInstanceBase::IsValidItemSlot(FSoulItemSlot ItemSlot) const
{
return false;
}

USoulSaveGame* USoulGameInstanceBase::GetCurrentSaveGame()
{
	return nullptr;
}

void USoulGameInstanceBase::SetSavingEnabled(bool bEnabled)
{
}

bool USoulGameInstanceBase::LoadOrCreateSaveGame()
{
	return false;
}

bool USoulGameInstanceBase::WriteSaveGame()
{
	return false;
}

void USoulGameInstanceBase::ResetSaveGame()
{
	bool bWasSavingEnabled = bSavingEnabled;
	bSavingEnabled = false;
	LoadOrCreateSaveGame();
	bSavingEnabled = bWasSavingEnabled;
}
