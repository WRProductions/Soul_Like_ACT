// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Inventory.h"
#include "UI/Widget_InventorySlot.h"
#include "Components/WrapBox.h"
#include "Player/InventoryManager.h"
#include "Player/Soul_Like_ACTCharacter.h"

void UWidget_Inventory::ConstructInventorySlots()
{
	if (!InventorySlot_BPClass)
	{
		UE_LOG(LogTemp, Error, TEXT("UWidget_Inventory::No Inventory Slot class"));
		return;
	}
	if (!GetOwningPlayerPawn())
	{
		UE_LOG(LogTemp, Error, TEXT("UWidget_Inventory::No Owning player"));
		return;
	}

	ASoul_Like_ACTCharacter* MyPlayer = Cast<ASoul_Like_ACTCharacter>(GetOwningPlayerPawn());
	UInventoryManager* LocalInventManager = MyPlayer->GetInventoryManager();

	//Load Inventory
	for (TPair<FSoulItemSlot, FSoulItemData> &LocalItemSlot : LocalInventManager->InventoryItems)
	{
		UWidget_InventorySlot *LocalSlot = Cast<UWidget_InventorySlot>(
			CreateWidget(GetOwningPlayer()
			, InventorySlot_BPClass));

		//Give Item pointer
		LocalSlot->SetupSlot(LocalItemSlot.Value);

		//Add to wrapper
		InventorySlotWrapper->AddChildToWrapBox(LocalSlot);
		InventorySlots.Add(LocalItemSlot.Key, LocalSlot);
	}

	//Load Equipment

	/*UE_LOG(LogTemp, Warning, TEXT("%s successful"), *FString(__FUNCTION__));*/
	LOG_FUNC_SUCCESS();
}

void UWidget_Inventory::BindWiddgets()
{
	ASoul_Like_ACTCharacter* MyPlayer = Cast<ASoul_Like_ACTCharacter>(GetOwningPlayerPawn());
	UInventoryManager* LocalInventManager = MyPlayer->GetInventoryManager();
	if (LocalInventManager)
	{
		LocalInventManager->OnSlottedItemChanged.AddDynamic(this, &UWidget_Inventory::UpdateInventSlot);
		LocalInventManager->OnEquipmentChangedChanged.AddDynamic(this, &UWidget_Inventory::UpdateGearSlot);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to bind on sloted changed"), *this->GetName());
	}
}

void UWidget_Inventory::UpdateInventSlot(FSoulItemSlot ItemSlot, FSoulItemData Item)
{
	UWidget_InventorySlot **LocalWidget = InventorySlots.Find(ItemSlot);

	(*LocalWidget)->SetupSlot(Item);
}

void UWidget_Inventory::UpdateGearSlot(FSoulEquipmentSlot EquipSlot, FSoulItemData Item)
{
	if (EquipSlot.SlotType == EGearType::Amulet)
	{
		EquipSlot_Amulet->SetupSlot(Item);
	}
	else if (EquipSlot.SlotType == EGearType::BodyArmor)
	{
		EquipSlot_Body->SetupSlot(Item);
	}
	else if (EquipSlot.SlotType == EGearType::Ring)
	{
		EquipSlot_Ring->SetupSlot(Item);
	}
	else if (EquipSlot.SlotType == EGearType::Helmet)
	{
		EquipSlot_Helmet->SetupSlot(Item);
	}
	else if (EquipSlot.SlotType == EGearType::Gloves)
	{
		EquipSlot_Gloves->SetupSlot(Item);
	}
	else if (EquipSlot.SlotType == EGearType::Boots)
	{
		EquipSlot_Boots->SetupSlot(Item);
	}
	else if (EquipSlot.SlotType == EGearType::Reliquary)
	{
		EquipSlot_Reliquary->SetupSlot(Item);
	}
	else if (EquipSlot.SlotType == EGearType::Weapon)
	{
		EquipSlot_Weapon->SetupSlot(Item);
	}
}