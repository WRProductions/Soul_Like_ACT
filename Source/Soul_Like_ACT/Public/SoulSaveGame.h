// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/SoulItemTypes.h"
#include "GameFramework/SaveGame.h"
#include "SoulSaveGame.generated.h"


/** Extra information about a URPGItem that is in a player's inventory */
USTRUCT(BlueprintType)
	struct SOUL_LIKE_ACT_API FSoulSaveItemData
{
	GENERATED_BODY()

	FSoulSaveItemData()
		: ItemCount(0)
	{}

	FSoulSaveItemData(FPrimaryAssetId InItemID, int32 InSlotNumber, int32 InItemCount, int32 InItemLevel,
	                  TArray<FPrimaryAssetId> InSlotedJewls)
		: ItemID(InItemID)
		  , ItemCount(InItemCount)
		  , ItemLevel(InItemLevel)
		  , SlotedJewels(InSlotedJewls)
	{
	}

	FSoulSaveItemData(FSoulItemData InItemData)
		: ItemID(InItemData.ItemBase->GetPrimaryAssetId())
		, ItemCount(InItemData.ItemCount)
		, ItemLevel(InItemData.ItemLevel)
	{
		for(auto Jewel : InItemData.SlotedJewls)
		{
			SlotedJewels.Add(Jewel->GetPrimaryAssetId());
		}
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGame)
	FPrimaryAssetId ItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGame)
	int32 ItemCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGame)
	int32 ItemLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SaveGame)
	TArray<FPrimaryAssetId> SlotedJewels;
};

/**
 * 
 */
UCLASS(BlueprintType)
	class SOUL_LIKE_ACT_API USoulSaveGame : public USaveGame
{
	GENERATED_BODY()


public:
	/** Constructor */
	USoulSaveGame()
	{ }

	/** User's unique id */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	FString UserId;

	/** Map of items to item data */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	TArray<FSoulSaveItemData> InventoryItemData;

	/** Map of slotted items */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	TMap<FSoulEquipmentSlot, FSoulSaveItemData> EquipedItemData;

	void ResetSaveGame();
};
