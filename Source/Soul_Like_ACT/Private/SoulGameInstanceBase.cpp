// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulGameInstanceBase.h"
#include "SoulSaveGame.h"
#include "Item/ItemBasic.h"
#include "Item/SoulAssetManager.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "SoulAssetManager.h"



USoulGameInstanceBase::USoulGameInstanceBase()
	: SaveSlot(TEXT("SaveGame"))
	, SaveUserIndex(0)
{
}

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

void USoulGameInstanceBase::AddDefaultInventory_Implementation(bool bRemoveExtra)
{
	// If we want to remove extra, clear out the existing inventokay
 	if (bRemoveExtra)
 	{
 		CurrentSaveGame->InventoryItemData.Reset();
		CurrentSaveGame->EquipedItemData.Reset();
 	}
}

bool USoulGameInstanceBase::LoadOrCreateSaveGame()
{
	// Drop reference to old save game, this will GC out
	CurrentSaveGame = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, SaveUserIndex) && bSavingEnabled)
	{
		CurrentSaveGame = Cast<USoulSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, SaveUserIndex));
		CurrentSaveGame->UserId.AppendInt(FMath::FRandRange(0, 1000));
		UE_LOG(LogTemp, Warning, TEXT("GI: Loaded Successful"));
	}

	if (CurrentSaveGame)
	{
		// Make sure it has any newly added default inventory
		AddDefaultInventory(false);
		return true;
	}
	else
	{
		// This creates it on demand
		CurrentSaveGame = Cast<USoulSaveGame>(UGameplayStatics::CreateSaveGameObject(USoulSaveGame::StaticClass()));
		if (CurrentSaveGame)
			AddDefaultInventory(true);
		else
			UE_LOG(LogTemp, Warning, TEXT("GI: Failed to create save game"));
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

	UE_LOG(LogTemp, Warning, TEXT("GI: Save Slot Reset SUCCESSFUL"));
}

USoulSaveGame* USoulGameInstanceBase::GetSaveSlot()
{
	if (!CurrentSaveGame)
	{
		CurrentSaveGame = Cast<USoulSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, SaveUserIndex));
		
		if (!CurrentSaveGame)
			return nullptr;
	}
	
	return CurrentSaveGame;
}

void USoulGameInstanceBase::OnStartGameClicked_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("GI: GAME START"));
}

void USoulGameInstanceBase::OnAsyncLoadingFinished(const TArray<UObject*>& Outp)
{

	for (UObject* a : Outp)
	{
		USoulItem* TempItem = Cast<USoulItem>(a);
		if (TempItem->IsValidLowLevel()) {
			UE_LOG(LogTemp, Warning, TEXT("GI: %s is added as the default item"), *a->GetName());

			FSoulItemData newData(TempItem, 1, 1);
			CurrentSaveGame->InventoryItemData.Add(newData);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("GI: Empty Slot"));
			CurrentSaveGame->InventoryItemData.Add(FSoulItemData());
		}
	}
}