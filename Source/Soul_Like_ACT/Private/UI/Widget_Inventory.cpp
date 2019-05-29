// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Inventory.h"
#include "Components/WrapBox.h"
#include "Player/InventoryManager.h"
#include "Player/Soul_Like_ACTCharacter.h"

void UWidget_Inventory::ConstructInventorySlots()
{
	if (!(InventorySlotTemplate))
	{
		UE_LOG(LogTemp, Error, TEXT("%s ERROR -> %s"), *FString(__FUNCTION__), *FString("Unable to get InventorySlot class"));
		return;
	}
	if (!GetOwningPlayerPawn())
	{
		LOG_FUNC_ERROR("No Owning Player");

		return;
	}

	ASoul_Like_ACTCharacter* MyPlayer = Cast<ASoul_Like_ACTCharacter>(GetOwningPlayerPawn());
	UInventoryManager* LocalInventManager = MyPlayer->GetInventoryManager();

	//Load Inventory
	for (TPair<FSoulItemSlot, FSoulItemData> &LocalItemSlot : LocalInventManager->InventoryItems)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "UWidget_Inventory::ConstructInventorySlots");

		UWidget_InventorySlot *LocalSlot = Cast<UWidget_InventorySlot>(
			CreateWidget(GetOwningPlayer()
			, InventorySlotTemplate));

		//Give Item pointer
		LocalSlot->SetupSlot(LocalItemSlot.Key, LocalItemSlot.Value);

		//Add to wrapper
		InventorySlotWrapper->AddChildToWrapBox(LocalSlot);
		InventorySlots.Add(LocalItemSlot.Key, LocalSlot);
	}
	//Load Equipment
	BindWiddgets();

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
		LOG_FUNC_SUCCESS();

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Failed to bind on sloted changed"), *this->GetName());
	}
}

void UWidget_Inventory::UpdateInventSlot(FSoulItemSlot ItemSlot, FSoulItemData Item)
{
	UWidget_InventorySlot **LocalWidget = InventorySlots.Find(ItemSlot);

	(*LocalWidget)->SetupSlot(ItemSlot, Item);
}

void UWidget_Inventory::UpdateGearSlot(FSoulEquipmentSlot EquipSlot, FSoulItemData Item)
{
	if (EquipSlot.SlotType == EGearType::Amulet)
	{
		EquipSlot_Amulet->SetupSlot(EquipSlot, Item);
	}
	else if (EquipSlot.SlotType == EGearType::BodyArmor)
	{
		EquipSlot_Body->SetupSlot(EquipSlot, Item);
	}
	else if (EquipSlot.SlotType == EGearType::Ring)
	{
		EquipSlot_Ring->SetupSlot(EquipSlot, Item);
	}
	else if (EquipSlot.SlotType == EGearType::Helmet)
	{
		EquipSlot_Helmet->SetupSlot(EquipSlot, Item);
	}
	else if (EquipSlot.SlotType == EGearType::Gloves)
	{
		EquipSlot_Gloves->SetupSlot(EquipSlot, Item);
	}
	else if (EquipSlot.SlotType == EGearType::Boots)
	{
		EquipSlot_Boots->SetupSlot(EquipSlot, Item);
	}
	else if (EquipSlot.SlotType == EGearType::Reliquary)
	{
		EquipSlot_Reliquary->SetupSlot(EquipSlot, Item);
	}
	else if (EquipSlot.SlotType == EGearType::Weapon)
	{
		EquipSlot_Weapon->SetupSlot(EquipSlot, Item);
	}
}