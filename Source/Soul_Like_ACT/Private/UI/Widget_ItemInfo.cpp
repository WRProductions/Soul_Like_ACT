// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_ItemInfo.h"
#include "Types/SoulItemTypes.h"
#include "Item/ItemBasic.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"

void UWidget_ItemInfo::MakeItemInfo_Implementation(USoulItem* ItemToRead, const FSoulItemData& ItemData)
{
	ItemName->SetText(FText::FromString(ItemToRead->ItemName));
	
	ItemType->SetText(FText::FromString(ItemToRead->ItemType.ToString()));

	ItemLevel->SetText(FText::FromString(FString::FromInt(ItemData.ItemLevel)));

	//Item level
	if (ItemData.ItemLevel <= 0)
	{
		ItemStat->SetVisibility(ESlateVisibility::Hidden);
		ItemStatPlus->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		ItemStat->SetVisibility(ESlateVisibility::Visible);
		ItemStatPlus->SetVisibility(ESlateVisibility::Visible);
		ItemStat->SetText(FText::FromString((ItemData.ItemLevel * 2).ToString()));
	}

	//Item Count
	if (ItemToRead->MaxCount == 1)
	{
		ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		ItemQuantity->SetVisibility(ESlateVisibility::Visible);
		ItemQuantity->(FText::FromString((ItemData.ItemCount).ToString()));
	}

	//Icon
	UMaterialInstanceDynamic* DynIcon = ItemIcon->GetDynamicMaterial();
	DynIcon->SetTextureParameterValue(Icon, ItemToRead->ItemIcon.)
}
