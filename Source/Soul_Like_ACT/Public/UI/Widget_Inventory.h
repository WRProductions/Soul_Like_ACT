// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Widget_Inventory.generated.h"

class UWidget_InventorySlot;

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API UWidget_Inventory : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
		UWidget_InventorySlot* EquipSlot_Helmet;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
		UWidget_InventorySlot* EquipSlot_Body;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
		UWidget_InventorySlot* EquipSlot_Boots;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
		UWidget_InventorySlot* EquipSlot_Gloves;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
		UWidget_InventorySlot* EquipSlot_Amulet;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
		UWidget_InventorySlot* EquipSlot_Ring;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
		UWidget_InventorySlot* EquipSlot_Weapon;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
		UWidget_InventorySlot* EquipSlot_Reliquary;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (BindWidget), Category = Units)
		class UWrapBox* InventorySlotWrapper;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Default)
	TSubclassOf<UWidget_InventorySlot> InventorySlot_BPClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Units)
	TArray<UWidget_InventorySlot*> InventorySlots;

	UFUNCTION(BlueprintCallable)
	void ConstructInventorySlots();

	UFUNCTION(BlueprintCallable)
		void BindWidgets();
}