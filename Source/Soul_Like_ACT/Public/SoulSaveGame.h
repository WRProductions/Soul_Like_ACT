// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Types/SoulItemTypes.h"
#include "GameFramework/SaveGame.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "SoulSaveGame.generated.h"


struct FWCSaveGameArchive : public FObjectAndNameAsStringProxyArchive
{
	FWCSaveGameArchive(FArchive& InInnerArchive)
		: FObjectAndNameAsStringProxyArchive(InInnerArchive, true)
	{
		ArIsSaveGame = true;
		ArNoDelta = true; // Optional, useful when saving/loading variables without resetting the level.
		// Serialize variables even if weren't modified and mantain their default values.
	}
};

/** Extra information about a URPGItem that is in a player's inventory */
USTRUCT(BlueprintType)
	struct SOUL_LIKE_ACT_API FSoulSaveItemData
{
	GENERATED_BODY()

	FSoulSaveItemData()
		: SlotNumber(-1)
	{
	}

	FSoulSaveItemData(FPrimaryAssetId InItemID, int32 InSlotNumber, int32 InItemCount, int32 InItemLevel,
	                  TArray<FPrimaryAssetId> InSlotedJewls)
		: ItemID(InItemID)
		  , SlotNumber(InSlotNumber)
		  , ItemCount(InItemCount)
		  , ItemLevel(InItemLevel)
		  , SlotedJewels(InSlotedJewls)
	{
	}

	FSoulSaveItemData(FSoulItemSlot InItemSlot, FSoulItemData InItemData)
		: ItemID(InItemData.ItemBase->GetPrimaryAssetId())
	, SlotNumber(InItemSlot.SlotNumber)
	, ItemCount(InItemData.ItemCount)
	, ItemLevel(InItemData.ItemLevel)
	{
		for(auto Jewel : InItemData.SlotedJewls)
		{
			SlotedJewels.Add(Jewel->GetPrimaryAssetId());
		}
	}

	FPrimaryAssetId ItemID;

	int32 SlotNumber;

	int32 ItemCount;

	int32 ItemLevel;

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
		: UserId("Fresh Bird ")
	{
		InventoryItemData.Init(FSoulItemData(), 40);
		EquipedItemData.Add(FSoulEquipmentSlot(EGearTypes::BodyArmor));
		EquipedItemData.Add(FSoulEquipmentSlot(EGearTypes::Helmet));
		EquipedItemData.Add(FSoulEquipmentSlot(EGearTypes::Weapon));
		EquipedItemData.Add(FSoulEquipmentSlot(EGearTypes::Legs));
		EquipedItemData.Add(FSoulEquipmentSlot(EGearTypes::Boots));
		EquipedItemData.Add(FSoulEquipmentSlot(EGearTypes::Ring));
		EquipedItemData.Add(FSoulEquipmentSlot(EGearTypes::Amulet));
		EquipedItemData.Add(FSoulEquipmentSlot(EGearTypes::Reliquary));

	}

	/** User's unique id */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	FString UserId;

	/** Map of items to item data */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	TArray<FSoulItemData> InventoryItemData;

	/** Map of slotted items */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = SaveGame)
	TMap<FSoulEquipmentSlot, FSoulItemData> EquipedItemData;
};
