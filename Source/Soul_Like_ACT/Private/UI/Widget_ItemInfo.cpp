// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_ItemInfo.h"
#include "Types/SoulItemTypes.h"
#include "Item/ItemBasic.h"
#include "Components/VerticalBox.h"

void UWidget_ItemInfo::MakeItemInfo_Implementation(USoulItem* ItemToRead, const FSoulItemData& ItemData)
{
	ItemType->SetText(FText::FromString(ItemToRead->ItemType.ToString()));
	ItemLevel->SetText(FText::FromString(FString::FromInt(ItemData.ItemLevel)));
	ItemStat->SetText(FText::FromString(FString::FromInt(2 * ItemData.ItemLevel)));
}
