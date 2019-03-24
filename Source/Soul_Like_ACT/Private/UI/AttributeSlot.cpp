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
}

void UAttributeSlot::OnAttributeChanged(TArray<float> values)
{
	if (MyAttribute == USoulAttributeSet::GetHealthAttribute()
		|| MyAttribute == USoulAttributeSet::GetStaminaAttribute())
	{
		FText localText = FText::Format(FText::FromString("%i/%i"), (int32)values[0], (int32)values[1]);
		AttributeValue->SetText(localText);
	}
	else if (MyAttribute == USoulAttributeSet::GetLeechAttribute()
		|| MyAttribute == USoulAttributeSet::GetMoveSpeedAttribute()
		|| MyAttribute == USoulAttributeSet::GetTenacityAttribute()
		|| MyAttribute == USoulAttributeSet::GetAttackPowerAttribute()
		|| MyAttribute == USoulAttributeSet::GetAttackSpeedAttribute()
		|| MyAttribute == USoulAttributeSet::GetDefensePowerAttribute())
	{
		AttributeValue->SetText(FText::FromString(FString::FromInt((int32)values[0])));
	}
}
