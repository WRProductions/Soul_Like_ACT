// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/AttributeSlot.h"
#include "Components/TextBlock.h"
#include "Abilities/SoulAttributeSet.h"
#include "Abilities/SoulAbilitySystemComponent.h"


void UAttributeSlot::SetAttributeType()
{
	if (MyAttribute == USoulAttributeSet::GetHealthAttribute())
		AttributeType->SetText(FText::FromString("Health:")); 

	else if (MyAttribute == USoulAttributeSet::GetStaminaAttribute())
		AttributeType->SetText(FText::FromString("Stamina:"));
	
	else if (MyAttribute == USoulAttributeSet::GetLeechAttribute())
		AttributeType->SetText(FText::FromString("Leech:"));

	else if (MyAttribute == USoulAttributeSet::GetMoveSpeedAttribute())
		AttributeType->SetText(FText::FromString("Move Speed:"));

	else if (MyAttribute == USoulAttributeSet::GetTenacityAttribute())
		AttributeType->SetText(FText::FromString("Tenacity:"));

	else if (MyAttribute == USoulAttributeSet::GetAttackPowerAttribute())
		AttributeType->SetText(FText::FromString("Attack Power:"));

	else if (MyAttribute == USoulAttributeSet::GetDefensePowerAttribute())
		AttributeType->SetText(FText::FromString("Defense:"));

	else if (MyAttribute == USoulAttributeSet::GetAttackSpeedAttribute())
		AttributeType->SetText(FText::FromString("Attack Speed:"));

	else if (MyAttribute == USoulAttributeSet::GetCriticalStrikeAttribute())
		AttributeType->SetText(FText::FromString("Critical Strike Chance:"));

	else if (MyAttribute == USoulAttributeSet::GetCriticalMultiAttribute())
		AttributeType->SetText(FText::FromString("Critical Multiplier:"));
}

void UAttributeSlot::OnAttributeChanged(const TArray<float> & values)
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

