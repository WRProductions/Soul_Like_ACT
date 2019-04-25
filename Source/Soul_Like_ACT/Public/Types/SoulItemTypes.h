// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBasic.h"
#include "SoulItemTypes.generated.h"

USTRUCT(BlueprintType)
struct SOUL_LIKE_ACT_API FSoulItemSlot
{
	GENERATED_BODY()

	/** Constructor, -1 means an invalid slot */
	FSoulItemSlot()
		: SlotNumber(-1)
	{
	}

	FSoulItemSlot(int32 InSlotNumber)
		: SlotNumber(InSlotNumber)
	{
	}

	/** The number of this slot, 0 indexed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 SlotNumber;

	/** Equality operators */
	bool operator==(const FSoulItemSlot& Other) const
	{
		return SlotNumber == Other.SlotNumber;
	}

	bool operator!=(const FSoulItemSlot& Other) const
	{
		return !(*this == Other);
	}

	/** Implemented so it can be used in Maps/Sets */
	friend uint32 GetTypeHash(const FSoulItemSlot& Key)
	{
		uint32 Hash = 0;

		Hash = HashCombine(Hash, (uint32)Key.SlotNumber);
		return Hash;
	}

	/** Returns true if slot is valid */
	bool IsValid() const
	{
		//return ItemType.IsValid() && SlotNumber >= 0;
		return SlotNumber >= 0;
	}
};

USTRUCT(BlueprintType)
struct SOUL_LIKE_ACT_API FSoulEquipmentSlot
{
	GENERATED_BODY()

	FSoulEquipmentSlot()
		: SlotType(EGearTypes::Weapon)
	{
	}

	FSoulEquipmentSlot(EGearTypes InSlotType)
		: SlotType(InSlotType)
	{
	}

	/** The number of this slot, 0 indexed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	EGearTypes SlotType;


	friend bool operator==(const FSoulEquipmentSlot& Lhs, const FSoulEquipmentSlot& RHS)
	{
		return Lhs.SlotType == RHS.SlotType;
	}

	friend bool operator!=(const FSoulEquipmentSlot& Lhs, const FSoulEquipmentSlot& RHS)
	{
		return !(Lhs == RHS);
	}

	friend uint32 GetTypeHash(const FSoulEquipmentSlot& OBJ)
	{
		uint32 Hash = 0;

		Hash = HashCombine(Hash, (uint32)OBJ.SlotType);
		return Hash;
	}
};

/** Extra information about a URPGItem that is in a player's inventory */
USTRUCT(BlueprintType)
struct SOUL_LIKE_ACT_API FSoulItemData
{
	GENERATED_BODY()

	/** Constructor, default to count/level 1 so declaring them in blueprints gives you the expected behavior */
	FSoulItemData()
		: ItemCount(1)
		  , ItemLevel(1)
	{
	}

	FSoulItemData(USoulItem* InItemBase)
		: ItemBase(InItemBase)
		, ItemCount(1)
		, ItemLevel(1)
	{}

	FSoulItemData(USoulItem* InItemBase, int32 InItemCount, int32 InItemLevel)
		: ItemBase(InItemBase)
		  , ItemCount(InItemCount)
		  , ItemLevel(InItemLevel)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	USoulItem* ItemBase;

	/** The number of instances of this item in the inventory, can never be below 1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ItemCount;

	/** The level of this item. This level is shared for all instances, can never be below 1 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 ItemLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Slots)
	TArray<USoulJewelItem*> SlotedJewls;

	/** Equality operators */
	bool operator==(const FSoulItemData& Other) const
	{
		return ItemCount == Other.ItemCount && ItemLevel == Other.ItemLevel && ItemBase == Other.ItemBase && SlotedJewls == Other.SlotedJewls;
	}

	bool operator!=(const FSoulItemData& Other) const
	{
		return !(*this == Other);
	}

	/** Returns true if count is greater than 0 */
	bool IsValid() const
	{
		return ItemCount > 0 && ItemBase->IsValidLowLevel();
	}

	/** Append an item data, this adds the count and overrides everything else */
	void UpdateItemData(const FSoulItemData& Other, int32 MaxCount, int32 MaxLevel)
	{
		if (MaxCount <= 0)
		{
			MaxCount = MAX_int32;
		}

		if (MaxLevel <= 0)
		{
			MaxLevel = MAX_int32;
		}

		ItemCount = FMath::Clamp(ItemCount + Other.ItemCount, 1, MaxCount);
		ItemLevel = FMath::Clamp(Other.ItemLevel, 1, MaxLevel);
	}
};

/** Delegate called when the contents of an inventory slot change */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlottedItemChanged, FSoulItemSlot, ItemSlot, FSoulItemData, Item);

/** Delegate called when the entire inventory has been loaded, all items may have been replaced */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryLoaded);
