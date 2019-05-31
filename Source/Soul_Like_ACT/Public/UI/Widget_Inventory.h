// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Soul_Like_ACT.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widget_InventorySlot.h"
#include "Widget_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UWidget_Inventory : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
		UWidget_EquipmentSlot* EquipSlot_Helmet;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
		UWidget_EquipmentSlot* EquipSlot_Body;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
		UWidget_EquipmentSlot* EquipSlot_Boots;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
		UWidget_EquipmentSlot* EquipSlot_Gloves;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
		UWidget_EquipmentSlot* EquipSlot_Amulet;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
		UWidget_EquipmentSlot* EquipSlot_Ring;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
		UWidget_EquipmentSlot* EquipSlot_Weapon;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
		UWidget_EquipmentSlot* EquipSlot_Reliquary;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
		class UWrapBox* InventorySlotWrapper;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Default)
		TSubclassOf<UWidget_InventorySlot> InventorySlotTemplate;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Units)
		TMap<FSoulInventSlot, UWidget_InventorySlot*> InventorySlots;

	UFUNCTION(BlueprintCallable)
		void ConstructInventorySlots();

	UFUNCTION(BlueprintCallable)
		void BindWiddgets();

	UFUNCTION()
		void UpdateInventSlot(FSoulInventSlot ItemSlot, FSoulItemData Item);
	UFUNCTION()
		void UpdateGearSlot(FSoulEquipmentSlot EquipSlot, FSoulItemData Item);
};