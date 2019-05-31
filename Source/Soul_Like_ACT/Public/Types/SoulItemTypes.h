// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Item/ItemBasic.h"
#include "SoulItemTypes.generated.h"


USTRUCT(BlueprintType)
struct SOUL_LIKE_ACT_API FSoulInventSlot
{
	GENERATED_BODY()

	/** Constructor, -1 means an invalid slot */
	FSoulInventSlot()
		: SlotNumber(-1)
	{
	}

	FSoulInventSlot(int32 InSlotNumber)
		: SlotNumber(InSlotNumber)
	{
	}

	/** The number of this slot, 0 indexed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	int32 SlotNumber;

	/** Equality operators */
	bool operator==(const FSoulInventSlot& Other) const
	{
		return SlotNumber == Other.SlotNumber;
	}

	bool operator!=(const FSoulInventSlot& Other) const
	{
		return !(*this == Other);
	}

	/** Implemented so it can be used in Maps/Sets */
	friend uint32 GetTypeHash(const FSoulInventSlot& Key)
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
		: SlotType(EGearType::Weapon)
	{
	}

	FSoulEquipmentSlot(EGearType InSlotType)
		: SlotType(InSlotType)
	{
	}

	/** The number of this slot, 0 indexed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Item)
	EGearType SlotType;


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

	FSoulItemData(USoulItem* InItemBase, int32 InItemCount, int32 InItemLevel)
		: ItemBase(InItemBase)
		, ItemCount(InItemCount)
		, ItemLevel(InItemLevel)
	{}

	/** Constructor, default to count/level 1 so declaring them in blueprints gives you the expected behavior */
	FSoulItemData()
		: FSoulItemData(nullptr, -1, -1)
	{}

	FSoulItemData(USoulItem* InItemBase)
		: FSoulItemData(ItemBase, 1, 1)
	{}

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

	/** Equality operators. It doesn't execute QUANTITY check */
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
	bool UpdateItemData(UPARAM(ref) FSoulItemData& Other)
	{
		if (ItemBase != Other.ItemBase || ItemLevel != Other.ItemLevel)
		{
			UE_LOG(LogTemp, Warning, TEXT("UpdateItemData failed, items don't match"));
			return false;
		}

		ItemCount = FMath::Clamp(ItemCount + Other.ItemCount, 1, ItemBase->MaxCount);

		int32 CountOverflow = ItemBase->MaxCount - ItemCount;

		if (CountOverflow < 0)
		{
			ItemCount = ItemBase->MaxCount;
			Other.ItemCount = CountOverflow;
		}
		else
		{
			Other.ItemCount = 0;
		}
		UE_LOG(LogTemp, Log, TEXT("UpdateItemData success, new_ItemCount: %i"), ItemCount);

		return true;
	}

	/** Return whether the item data and input has the same item type and ilv */
	bool HasSameItem(FSoulItemData& Other) const
	{
		return ItemLevel == Other.ItemLevel && ItemBase == Other.ItemBase && SlotedJewls == Other.SlotedJewls;
	}

	FString ToString() const
	{
		if (IsValid())
		{
			FString newString;
			newString.Append(ItemBase->GetName() + ", Count: ");
			newString.AppendInt(ItemCount);
			newString.Append(", Level: ");
			newString.AppendInt(ItemLevel);
			newString.Append(", JewelsNum: ");
			newString.AppendInt(SlotedJewls.Num());

			return newString;
		}
		else
		{
			return "Invalid SoulItem";
		}
	}

};


/** Delegate called when the contents of an inventory slot change */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSlottedItemChanged, FSoulInventSlot, ItemSlot, FSoulItemData, Item);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnEquipmentChanged, FSoulEquipmentSlot, EquipmentSlot, FSoulItemData, Item);

