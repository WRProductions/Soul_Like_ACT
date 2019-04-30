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
	static ConstructorHelpers::FClassFinder<USoulSaveGame> DefaultSaveGame(TEXT("/Game/Blueprints/SoulSaveGame"));
	DefaultSaveGameBPClass = DefaultSaveGame.Class;
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
		//CurrentSaveGame->ResetSaveGame();
 	}
}

bool USoulGameInstanceBase::LoadOrCreateSaveGame()
{
	// Drop reference to old save game, this will GC out
	CurrentSaveGame = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, SaveUserIndex) && !bForceReset)
	{
		CurrentSaveGame = Cast<USoulSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, SaveUserIndex));
	}

	if (CurrentSaveGame)
	{
		// Make sure it has any newly added default inventory
		AddDefaultInventory(false);

		UE_LOG(LogTemp, Warning, TEXT("GI: Loaded Successful"));
		return true;
	}
	else
	{
		// This creates it on demand
		CurrentSaveGame = Cast<USoulSaveGame>(UGameplayStatics::CreateSaveGameObject(DefaultSaveGameBPClass));
		WriteSaveGame();
		if (CurrentSaveGame)
		{
			AddDefaultInventory(true);
			UE_LOG(LogTemp, Warning, TEXT("GI: New Save Slot"));
		}
		else
			UE_LOG(LogTemp, Warning, TEXT("GI: Failed to create save game"));
		return false;
	}
}

bool USoulGameInstanceBase::WriteSaveGame()
{
	LOG_FUNC_SUCCESS();

	return UGameplayStatics::SaveGameToSlot(CurrentSaveGame, SaveSlot, SaveUserIndex);
}

void USoulGameInstanceBase::ResetSaveGame()
{
	bForceReset = true;
	LoadOrCreateSaveGame();
	bForceReset = false;

	LOG_FUNC_SUCCESS();
}

void USoulGameInstanceBase::GetSoulPlayer(UObject* WorldContextObject, ASoulPlayerController*& MyController, ASoul_Like_ACTCharacter*& MyChar, UInventoryManager*& MyInentory, bool &Successful)
{
	UWorld* MyWorld = WorldContextObject->GetWorld();
	
	MyController = Cast<ASoulPlayerController>(MyWorld->GetFirstPlayerController());
	
	if (MyController)
	{
		MyChar = Cast<ASoul_Like_ACTCharacter>(MyController->GetPawn());
		if (MyChar)
		{
			MyInentory = MyChar->GetInventoryManager();
			Successful = true;
			return;
		}
	}
	Successful = false;
	return;
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
	WriteSaveGame();
	UE_LOG(LogTemp, Warning, TEXT("GI: GAME START"));
}

void USoulGameInstanceBase::OnAsyncLoadingFinished(const TArray<UObject*>& Outp)
{
	uint8 i = 0;
	for (UObject* a : Outp)
	{
		USoulItem* TempItem = Cast<USoulItem>(a);
		if (TempItem->IsValidLowLevel()) {
			UE_LOG(LogTemp, Warning, TEXT("GI: %s is added as the default item"), *a->GetName());

			FSoulItemData newData(TempItem, 1, 1);
			/*CurrentSaveGame->InventoryItemData[i] = newData;*/
			CurrentSaveGame->InventoryItemData[i] = newData;
		}
		++i;
	}

	UE_LOG(LogTemp, Warning, TEXT("%s save game has %d invent slots"), *(FString(__FUNCTION__)), CurrentSaveGame->InventoryItemData.Num());
	UE_LOG(LogTemp, Warning, TEXT("%s save game has %d Equip slots"), *(FString(__FUNCTION__)), CurrentSaveGame->EquipedItemData.Num());

	if (OnSaveGameLoadingFinished.IsBound())
		OnSaveGameLoadingFinished.Broadcast();
}