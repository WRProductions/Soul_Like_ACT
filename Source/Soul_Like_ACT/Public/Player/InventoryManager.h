// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "Types/SoulItemTypes.h"
#include "Components/ActorComponent.h"
#include "InventoryManager.generated.h"

class AWeaponActor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnInventoryLoadingFinished, UInventoryManager*, OutInventoryManager, bool, OutFirstTimeInventLoading);

/**
 * This component is only been used to handle Items
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API UInventoryManager : public UActorComponent
{
	GENERATED_BODY()

	class ASoul_Like_ACTCharacter* PlayerRef;

public:
	UInventoryManager();

/**
 * Inventory
 */
protected:
	/** Adds a new inventory item, will add it to an empty slot if possible.
	If the item supports count you can add more than one count.
	It will also update the level when adding if required */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool AddInventoryItem(FSoulItemData InItemData);
	
	/** Remove an inventory item, will also remove from slots.
	A remove count of <= 0 means to remove all copies */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool RemoveInventoryItem(FSoulItemData RemovedItem);
	
	/** Remove an inventory item */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool RemoveInventoryItemAtIndex(FSoulItemSlot InItemSlot, int32 ItemCount = 1);


	//************************************
	// Method:    
	// Parameter: FSoulItemData InItemData
	// Parameter: FSoulItemSlot & OutSlot
	// Parameter: bSkipFullSlot Whether will skip the full stacked slot
	// Parameter: bGetEmptySlot Whether can return a empty slot
	//************************************
	bool GetFirstSlot(FSoulItemData InItemData, FSoulItemSlot & OutSlot, bool bSkipFullSlot = true, bool bGetEmptySlot = false) const;

	//************************************
	// Method:    Returns all inventory items of a given type. If none is passed as type it will return all
	// Parameter: FSoulItemData InItemData
	// Parameter: TArray<FSoulItemSlot> & OutItemDatas
	// Parameter: bSkipFullSlot Whether will skip the full stacked slot
	// Parameter: bGetEmptySlot Whether can return a empty slot
	//************************************
	bool GetSlots(FSoulItemData InItemData, TArray<FSoulItemSlot>& OutItemDatas, bool bSkipFullSlot = true, bool bGetEmptySlot = false) const;

	/** Get the reference of the ItemData at the slot. Return true if the data has a valid ItemBase and positive quantity*/
	UFUNCTION(BlueprintPure, Category = Inventory)
	bool GetInventoryItemData(FSoulItemSlot InItemSlot, FSoulItemData& ItemData) const;

	/** Returns number of instances of this item found in the inventory. This uses count from GetItemData */
	UFUNCTION(BlueprintPure, Category = Inventory)
	int32 GetSlottedItemCount(FSoulItemSlot InItemSlot) const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	const EGearType GetGearType(FSoulItemSlot InItemSlot);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void AddEquipment(FSoulItemSlot InventorySlot);

/**
 * Save/Load
 */
public:
	/** Map of slot, from type/num to item, initialized from ItemSlotsPerType on RPGGameInstanceBase */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FSoulItemSlot, FSoulItemData> InventoryItems;

	/** Map of slot, from type/num to item, initialized from ItemSlotsPerType on RPGGameInstanceBase */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FSoulEquipmentSlot, FSoulItemData> EquipedItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AWeaponActor* CurrentWeapon;

	UFUNCTION(BlueprintCallable)
	void EquipGear(AWeaponActor * const Inp);

	/** Manually save the inventory, this is called from add/remove functions automatically */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool SaveInventory();

	/** It's used for Game Instance when async loading finshed */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool LoadInventoryData(TArray<FSoulItemData> InInventoryItems
		, TMap<FSoulEquipmentSlot, FSoulItemData> InEquipedItems);

	/** Delegate called when an inventory slot has changed */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
		FOnSlottedItemChanged OnSlottedItemChanged;
	UPROPERTY(BlueprintAssignable, Category = Inventory)
		FOnEquipmentChanged OnEquipmentChangedChanged;
	UPROPERTY(BlueprintAssignable, Category = Inventory)
		FOnInventoryLoadingFinished OnInventoryLoadingFinished;

protected:

	void SetItemSlot(FSoulItemData& InItemData, FSoulItemSlot ItemSlot);

	void Notify_OnInventoryLoadingFinished(bool bFirstTimeInit);

	/** Calls the inventory update callbacks */
	void NotifySlottedItemChanged(FSoulItemSlot ItemSlot, FSoulItemData ItemData);
	void NotifyEquipmentChanged(FSoulEquipmentSlot EquipmentSlot, FSoulItemData ItemData);

	friend ASoul_Like_ACTCharacter;
};