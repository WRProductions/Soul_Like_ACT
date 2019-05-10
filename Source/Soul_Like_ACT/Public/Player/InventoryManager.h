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

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	UInventoryManager();

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

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool LoadInventoryData(TMap<FSoulItemSlot, FSoulItemData> InInventoryItems
		, TMap<FSoulEquipmentSlot, FSoulItemData> InEquipedItems);

	/** Delegate called when an inventory slot has changed */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
		FOnSlottedItemChanged OnSlottedItemChanged;
	UPROPERTY(BlueprintAssignable, Category = Inventory)
		FOnEquipmentChanged OnEquipmentChangedChanged;
	UPROPERTY(BlueprintAssignable, Category = Inventory)
		FOnInventoryLoadingFinished OnInventoryLoadingFinished;

protected:
	void Notify_OnInventoryLoadingFinished(bool bFirstTimeInit);

	/** Auto slots a specific item, returns true if anything changed */
	bool FillEmptySlotWithItem(FSoulItemData NewItemData);

	/** Calls the inventory update callbacks */
	void NotifySlottedItemChanged(FSoulItemSlot ItemSlot, FSoulItemData ItemData);
	void NotifyEquipmentChanged(FSoulEquipmentSlot EquipmentSlot, FSoulItemData ItemData);
};