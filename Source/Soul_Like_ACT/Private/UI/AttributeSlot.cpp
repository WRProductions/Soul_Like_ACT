// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/AttributeSlot.h"
#include "Components/TextBlock.h"
#include "Abilities/SoulAttributeSet.h"
#include "Abilities/SoulAbilitySystemComponent.h"

void UAttributeSlot::SetAttributeType()
{
	if (MyAttribute == AttributeSetRef->GetHealthAttribute())
		AttributeType->SetText(FText::FromString("Health:"));

	else if (MyAttribute == AttributeSetRef->GetStaminaAttribute())
		AttributeType->SetText(FText::FromString("Stamina:"));
	
	else if (MyAttribute == AttributeSetRef->GetLeechAttribute())
		AttributeType->SetText(FText::FromString("Leech:"));

	else if (MyAttribute == AttributeSetRef->GetMoveSpeedAttribute())
		AttributeType->SetText(FText::FromString("Move Speed:"));

	else if (MyAttribute == AttributeSetRef->GetTenacityAttribute())
		AttributeType->SetText(FText::FromString("Tenacity:"));

	else if (MyAttribute == AttributeSetRef->GetAttackPowerAttribute())
		AttributeType->SetText(FText::FromString("Attack Power:"));

	else if (MyAttribute == AttributeSetRef->GetDefensePowerAttribute())
		AttributeType->SetText(FText::FromString("Defense:"));

	else if (MyAttribute == AttributeSetRef->GetAttackSpeedAttribute())
		AttributeType->SetText(FText::FromString("Attack Speed:"));
}

void UAttributeSlot::OnAttributeChanged()
{
	if (MyAttribute == AttributeSetRef->GetHealthAttribute())
	{
		FText localText = FText::Format(FText::FromString("%i/%i"), (int32)AttributeSetRef->GetHealth(), (int32)AttributeSetRef->GetMaxHealth());
		AttributeValue->SetText(localText);
	}
	else if (MyAttribute == AttributeSetRef->GetStaminaAttribute())
	{
		FText localText = FText::Format(FText::FromString("%i/%i"), (int32)AttributeSetRef->GetStamina(), (int32)AttributeSetRef->GetMaxStamina());
		AttributeValue->SetText(localText);
	}
	else if (MyAttribute == AttributeSetRef->GetLeechAttribute())
	{
		AttributeValue->SetText(FText::FromString(FString::FromInt((int)AttributeSetRef->GetLeech())));
	}
	else if (MyAttribute == AttributeSetRef->GetMoveSpeedAttribute())
	{
		AttributeValue->SetText(FText::FromString(FString::FromInt((int)AttributeSetRef->GetMoveSpeed())));
	}
	else if (MyAttribute == AttributeSetRef->GetTenacityAttribute())
	{
		AttributeValue->SetText(FText::FromString(FString::FromInt((int)AttributeSetRef->GetTenacity())));
	}
	else if (MyAttribute == AttributeSetRef->GetAttackPowerAttribute())
	{
		AttributeValue->SetText(FText::FromString(FString::FromInt((int)AttributeSetRef->GetAttackPower())));
	}
	else if (MyAttribute == AttributeSetRef->GetAttackSpeedAttribute())
	{
		AttributeValue->SetText(FText::FromString(FString::FromInt((int)AttributeSetRef->GetAttackSpeed())));
	}
	else if (MyAttribute == AttributeSetRef->GetDefensePowerAttribute())
	{
		AttributeValue->SetText(FText::FromString(FString::FromInt((int)AttributeSetRef->GetDefensePower())));
	}
}
