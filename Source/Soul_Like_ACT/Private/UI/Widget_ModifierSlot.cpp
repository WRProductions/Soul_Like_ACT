// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget_ModifierSlot.h"
#include "Item/ItemBasic.h"
#include "Components/TextBlock.h"
#include "BpFunctionLib/SoulSerializerBpLib.h"

void UWidget_ModifierSlot::Setup(const FString& inp_Name, const FString& inp_Level)
{
	GA_Name->SetText(FText::FromString(inp_Name));

	GA_Level->SetText(FText::FromString(inp_Level));
}
