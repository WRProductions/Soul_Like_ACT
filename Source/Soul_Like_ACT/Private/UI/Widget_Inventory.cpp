// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_Inventory.h"
#include "Components/WrapBox.h"

void UWidget_Inventory::ConstructInventorySlots()
{
	if (!InventorySlot_BPClass.Class)
		return;

	for (int i = 0; i < 40; i++)
	{
		UWidget_InventorySlot *LocalSlot = Cast<UWidget_InventorySlot>(
			CreateWidget(UGameplayStatics::GetPlayerController(GetWorld(), 0)
			, InventorySlot_BPClass));

		InventorySlotWrapper->AddChildToWrapBox(LocalSlot);
		InventorySlots.Add(LocalSlot);
	}
}
