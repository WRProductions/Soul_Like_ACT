// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/AttributeSlot.h"
#include "Components/TextBlock.h"
#include "Abilities/SoulAttributeSet.h"
#include "Abilities/SoulAbilitySystemComponent.h"
#include "SoulSerializerBpLib.h"


void UAttributeSlot::SetAttributeType()
{
	FString TempTypeString;
	USoulSerializerBpLib::AttributeToString(MyAttribute, TempTypeString);

	TempTypeString += ":";

	AttributeType->SetText(FText::FromString(TempTypeString));
}

void UAttributeSlot::OnAttributeChanged(const TArray<float>& values)
{
	if (MyAttribute == USoulAttributeSet::GetHealthAttribute()
		|| MyAttribute == USoulAttributeSet::GetStaminaAttribute())
	{
		FFormatOrderedArguments Args;
		Args.Add((int32)values[0]);
		Args.Add((int32)values[1]);
		FText localText = FText::Format(NSLOCTEXT("OnAttributeChanging", "Chanding1", "{0}/{1}"), Args);
		AttributeValue->SetText(localText);
	}
	else if (MyAttribute == USoulAttributeSet::GetLeechAttribute()
		|| MyAttribute == USoulAttributeSet::GetMoveSpeedAttribute()
		|| MyAttribute == USoulAttributeSet::GetTenacityAttribute()
		|| MyAttribute == USoulAttributeSet::GetAttackPowerAttribute()
		|| MyAttribute == USoulAttributeSet::GetDefensePowerAttribute())
	{
		AttributeValue->SetText(FText::FromString(FString::FromInt((int32)values[0])));
	}
	else if (MyAttribute == USoulAttributeSet::GetAttackSpeedAttribute()
		|| MyAttribute == USoulAttributeSet::GetCriticalStrikeAttribute())
	{
		FFormatOrderedArguments Args;
		Args.Add((int32)values[0]);
		FText localText = FText::Format(NSLOCTEXT("OnAttributeChanging", "Changing2", "{0}%"), Args);
		AttributeValue->SetText(localText);
	}
	else if (MyAttribute == USoulAttributeSet::GetCriticalMultiAttribute())
	{
		FFormatOrderedArguments Args;
		Args.Add((int32)values[0] + 100);
		FText localText = FText::Format(NSLOCTEXT("OnAttributeChanging", "Changing2", "{0}%"), Args);
		AttributeValue->SetText(localText);
	}
}
