// Fill out your copyright notice in the Description page of Project Settings.


#include "ModifierSlot.h"
#include "Item/ItemBasic.h"
#include "Components/TextBlock.h"
#include "BpFunctionLib/SoulSerializerBpLib.h"

void UModifierSlot::Setup(FString& inp_Name, FString& inp_Level)
{
	GA_Name->SetText(FText::FromString(inp_Name));

	GA_Level->SetText(FText::FromString(inp_Level));
}
