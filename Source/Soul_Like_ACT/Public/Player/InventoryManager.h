// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "Components/ActorComponent.h"
#include "InventoryManager.generated.h"

class AWeaponActor;
class USoulItem;
struct FSoulItemData;
struct FSoulItemSlot;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API UInventoryManager : public UActorComponent
{
	GENERATED_BODY()

	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	/** Map of all items owned by this player, from definition to data */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
		TMap<USoulItem*, FSoulItemData> InventoryData;

	/** Map of slot, from type/num to item, initialized from ItemSlotsPerType on RPGGameInstanceBase */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
		TMap<FSoulItemSlot, USoulItem*> SlottedItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AWeaponActor* CurrentWeapon;

public:	
	// Sets default values for this component's properties
	UInventoryManager();

	UFUNCTION(BlueprintCallable)
	void EquipGear(AWeaponActor * const Inp);

	const TMap<USoulItem*, FSoulItemData>& GetInventoryDataMap() const
	{
		return InventoryData;
	}
	const TMap<FSoulItemSlot, USoulItem*>& GetSlottedItemMap() const
	{
		return SlottedItems;
	}

	/** Delegate called when an inventory item has been added or removed */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnInventoryItemChanged OnInventoryItemChanged;

	/** Delegate called when an inventory slot has changed */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnSlottedItemChanged OnSlottedItemChanged;

	/** Delegate called when the inventory has been loaded/reloaded */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnInventoryLoaded OnInventoryLoaded;

	/** Called after the inventory was changed and we notified all delegates */
	UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
	void InventoryItemChanged(bool bAdded, USoulItem* Item);

	/** Called after an item was equipped and we notified all delegates */
	UFUNCTION(BlueprintImplementableEvent, Category = Inventory)
	void SlottedItemChanged(FSoulItemSlot ItemSlot, USoulItem* Item);

	/** Adds a new inventory item, will add it to an empty slot if possible. If the item supports count you can add more than one count. It will also update the level when adding if required */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool AddInventoryItem(USoulItem* NewItem, int32 ItemCount = 1, int32 ItemLevel = 1, bool bAutoSlot = true);

	/** Remove an inventory item, will also remove from slots. A remove count of <= 0 means to remove all copies */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool RemoveInventoryItem(USoulItem* RemovedItem, int32 RemoveCount = 1);

	/** Returns all inventory items of a given type. If none is passed as type it will return all */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void GetInventoryItems(TArray<USoulItem*>& Items, FPrimaryAssetType ItemType);

	/** Returns number of instances of this item found in the inventory. This uses count from GetItemData */
	UFUNCTION(BlueprintPure, Category = Inventory)
	int32 GetInventoryItemCount(USoulItem* Item) const;

	/** Returns the item data associated with an item. Returns false if none found */
	UFUNCTION(BlueprintPure, Category = Inventory)
	bool GetInventoryItemData(USoulItem* Item, FSoulItemData& ItemData) const;

	/** Sets slot to item, will remove from other slots if necessary. If passing null this will empty the slot */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool SetSlottedItem(FSoulItemSlot ItemSlot, USoulItem* Item);

	/** Returns item in slot, or null if empty */
	UFUNCTION(BlueprintPure, Category = Inventory)
	USoulItem* GetSlottedItemWithSlot(FSoulItemSlot ItemSlot) const;

	/** Returns all slotted items of a given type. If none is passed as type it will return all */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void GetSlottedItemsWithType(TArray<USoulItem*>& Items, FPrimaryAssetType ItemType, bool bOutputEmptyIndexes);

	/** Fills in any empty slots with items in inventory */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	void FillEmptySlots();

	/** Manually save the inventory, this is called from add/remove functions automatically */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool SaveInventory();

	/** Loads inventory from save game on game instance, this will replace arrays */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool LoadInventory();


protected:
	/** Auto slots a specific item, returns true if anything changed */
	bool FillEmptySlotWithItem(USoulItem* NewItem);

	/** Calls the inventory update callbacks */
	void NotifyInventoryItemChanged(bool bAdded, USoulItem* Item);
	void NotifySlottedItemChanged(FSoulItemSlot ItemSlot, USoulItem* Item);
	void NotifyInventoryLoaded();
};
