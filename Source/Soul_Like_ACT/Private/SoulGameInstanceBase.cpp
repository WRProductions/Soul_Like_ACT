// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulGameInstanceBase.h"
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

void USoulGameInstanceBase::AddDefaultInventory()
{
	for (int i = 0; i < DefaultInventory.Num(); i++)
	{
		CurrentSaveGame->InventoryItemData[i] = DefaultInventory[i];
	}
}

bool USoulGameInstanceBase::LoadOrCreateSaveGame()
{
	// Drop reference to old save game, this will GC out
	CurrentSaveGame = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SaveSlot, SaveUserIndex) && !bForceReset)
	{
		CurrentSaveGame = Cast<USoulSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlot, SaveUserIndex));
		UE_LOG(LogTemp, Warning, TEXT("GI: Loading"));
	}

	//Old Slot
	if (CurrentSaveGame)
	{
		UE_LOG(LogTemp, Warning, TEXT("GI: Loaded Successful"));
		WriteSaveGame();
		Broadcast_OnSaveGameLoadFinshed();
		return true;
	}
	else
	{
		//New Slot
		CurrentSaveGame = Cast<USoulSaveGame>(UGameplayStatics::CreateSaveGameObject(DefaultSaveGameBPClass));
		if (CurrentSaveGame)
		{
			AddDefaultInventory();
			WriteSaveGame();
			
			Broadcast_OnSaveGameLoadFinshed();
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
	LOG_FUNC_SUCCESS();

	bForceReset = true;
	LoadOrCreateSaveGame();
	bForceReset = false;
}

void USoulGameInstanceBase::MakeSaveData()
{
	APawn *TempPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	
	if (!TempPawn || !Cast<ASoul_Like_ACTCharacter>(TempPawn))
	{
		LOG_FUNC_FAIL();
		return;
	}

	UInventoryManager* MyInventoryManager = Cast<ASoul_Like_ACTCharacter>(TempPawn)->GetInventoryManager();

}

void USoulGameInstanceBase::MakeSoulItemSaveData(FSoulItemData InItemData, FSoulSaveItemData& OutSaveItemData)
{
	OutSaveItemData = FSoulSaveItemData(InItemData);
}

void USoulGameInstanceBase::MakeSoulItemData(UObject* InItemBase, TArray<UObject*> InJewels, FSoulItemData& OutItemData, int32 InItemCount /*= 1*/, int32 InItemLevel /*= 1*/)
{
	OutItemData = FSoulItemData(Cast<USoulItem>(InItemBase), InItemCount, InItemLevel);
	for (UObject* Jew : InJewels)
	{
		OutItemData.SlotedJewls.Add(Cast<USoulJewelItem>(InItemBase));
	}
}

void USoulGameInstanceBase::Broadcast_OnSaveGameLoadFinshed()
{
	if (OnSaveGameLoadingFinished.IsBound())
		OnSaveGameLoadingFinished.Broadcast();
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
