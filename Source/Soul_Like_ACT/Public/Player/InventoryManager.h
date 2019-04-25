// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponActor.h"
#include "Types/SoulItemTypes.h"
#include "Components/ActorComponent.h"
#include "InventoryManager.generated.h"

class AWeaponActor;
/**
 * This component is only been used to handle Items
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API UInventoryManager : public UActorComponent
{
	GENERATED_BODY()

	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	/** Map of slot, from type/num to item, initialized from ItemSlotsPerType on RPGGameInstanceBase */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FSoulItemSlot, FSoulItemData> InventoryItems;

		/** Map of slot, from type/num to item, initialized from ItemSlotsPerType on RPGGameInstanceBase */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory)
	TMap<FSoulEquipmentSlot, FSoulItemData> EquipedItems;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AWeaponActor* CurrentWeapon;

public:	
	// Sets default values for this component's properties
	UInventoryManager();

	UFUNCTION(BlueprintCallable)
	void EquipGear(AWeaponActor * const Inp);

	/** Delegate called when an inventory slot has changed */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnSlottedItemChanged OnSlottedItemChanged;
	/** Delegate called when the inventory has been loaded/reloaded */
	UPROPERTY(BlueprintAssignable, Category = Inventory)
	FOnInventoryLoaded OnInventoryLoaded;

	/** Manually save the inventory, this is called from add/remove functions automatically */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool SaveInventory();

	/** Loads inventory from save game on game instance, this will replace arrays */
	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool LoadInventory();

protected:
	/** Auto slots a specific item, returns true if anything changed */
	bool FillEmptySlotWithItem(FSoulItemData NewItemData);

	/** Calls the inventory update callbacks */
	void NotifySlottedItemChanged(FSoulItemSlot ItemSlot, FSoulItemData ItemData);
	void NotifyInventoryLoaded();
};