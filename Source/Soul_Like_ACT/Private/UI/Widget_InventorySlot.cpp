// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget_InventorySlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Materials/MaterialInstance.h"
#include "Player/InventoryManager.h"

void UWidget_InventorySlot::SetupSlot(FSoulInventSlot InItemSlot, FSoulItemData InItemData)
{
	USoulItem* ItemBase = InItemData.ItemBase;

	if (ItemBase && InItemData.ItemCount > 0)
	{
		item_icon->SetVisibility(ESlateVisibility::Visible);
		item_icon->GetDynamicMaterial()->SetTextureParameterValue(FName("IconTexture"), InItemData.ItemBase->ItemIcon);
		if (InItemData.ItemCount > 1)
		{
			item_quantity->SetText(FText::FromString(FString::FromInt(InItemData.ItemCount)));
			item_quantity->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			item_quantity->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	else
	{
		item_icon->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UWidget_EquipmentSlot::SetupSlot(FSoulEquipmentSlot InItemSlot, FSoulItemData InItemData)
{
	USoulItem* ItemBase = InItemData.ItemBase;

	if (ItemBase && InItemData.ItemCount == 1)
	{
		item_icon->SetVisibility(ESlateVisibility::Visible);
		item_icon->GetDynamicMaterial()->SetTextureParameterValue(FName("IconTexture"), InItemData.ItemBase->ItemIcon);
	}
	else
	{
		item_icon->SetVisibility(ESlateVisibility::Hidden);
	}
}
