// Fill out your copyright notice in the Description page of Project Settings.


#include "SoulSerializerBpLib.h"

bool USoulSerializerBpLib::GetModifiersFromItemBase(
	const USoulItem* ItemRef
	, TArray<FText>& ModifierNames
	, TArray<FText>& ModifierLevels)
{
	const TMap<TSubclassOf<USoulModifierGameplayAbility>, int32>& TargetGA = ItemRef->Modifiers;
	int32 GA_Size = TargetGA.Num();
	if (GA_Size <= 0)
	{
		return false;
	}
	else
	{
		uint8 ModifierNum = ModifierLevels.Num();
		uint8 i = ModifierNum;

		ModifierLevels.SetNum(ModifierNum + GA_Size, false);
		ModifierNames.SetNum(ModifierNum + GA_Size, false);

		for (auto &Element : TargetGA)
		{
			ModiferToText(Element, ModifierNames[i], ModifierLevels[i]);

			++i;
		}

		return ModifierNames.Num() > 0;
	}
}

bool USoulSerializerBpLib::GetModifiersFromItemData(
	const FSoulItemData& InItemData
	, TArray<FText>& ModifierNames
	, TArray<FText>& ModifierLevels
	, TArray<FText>& SlotModNames
	, TArray<FText>& SlotModLevels
	, int32& EmptySlots)
{
	GetModifiersFromItemBase(InItemData.ItemBase, ModifierNames, ModifierLevels);

	for (USoulJewelItem* LocalJewel : InItemData.SlotedJewls)
		GetModifiersFromItemBase(LocalJewel, SlotModNames, SlotModLevels);

	EmptySlots = InItemData.ItemBase->MaxSlots - SlotModNames.Num();

	return ModifierNames.Num() > 0;
}

void USoulSerializerBpLib::GetPrimaryStatusFromItem(const USoulItem* ItemRef, TArray<FText>& ModifierNames, TArray<FText>& ModifierLevels, bool& Successful)
{
	if (!ItemRef->PrimaryAbility)
	{
		Successful = 0;
		return;
	}

	USoulPrimaryStatusGameplayAbility *TempAbility = Cast<USoulPrimaryStatusGameplayAbility>(ItemRef->PrimaryAbility->GetDefaultObject(true));

	if (!TempAbility)
	{
		Successful = 0;
		return;
	}
	ModifierNames.Add(TempAbility->DisplayName);

	ModifierLevels.Add(FText::FromString(FString::FromInt(ItemRef->PrimaryAbilityLevel)));

	Successful = ModifierNames.Num() > 0;
}

void USoulSerializerBpLib::ModiferToText(const TPair<TSubclassOf<USoulModifierGameplayAbility>, int32>& InputAbilityInfo,
                                           FText& ModifierName, FText& ModifierLevel)
{
	USoulModifierGameplayAbility* TempGA = Cast<USoulModifierGameplayAbility>((InputAbilityInfo.Key)->GetDefaultObject(true));

	if (!TempGA)
	{
		return;
	}

	ModifierName = TempGA->DisplayName;
	ModifierLevel =  FText::FromString(FString::FromInt(InputAbilityInfo.Value));
}

void USoulSerializerBpLib::AttributeToString(FGameplayAttribute Attribute, FString& Output)
{
	if (Attribute == USoulAttributeSet::GetHealthAttribute())
		Output = "Health";
	else if (Attribute == USoulAttributeSet::GetPostureAttribute())
		Output = "Posture";
	else if (Attribute == USoulAttributeSet::GetLeechAttribute())
		Output = "Leech";
	else if (Attribute == USoulAttributeSet::GetMeleeRangeAttribute())
		Output = "Attack Range";
	else if (Attribute == USoulAttributeSet::GetMoveSpeedAttribute())
		Output = "Move Speed";
	else if (Attribute == USoulAttributeSet::GetPostureStrengthAttribute())
		Output = "Posture Strength";
	else if (Attribute == USoulAttributeSet::GetDefensePowerAttribute())
		Output = "Defense Power";
	else if (Attribute == USoulAttributeSet::GetAttackPowerAttribute())
		Output = "Attack Power";
	else if (Attribute == USoulAttributeSet::GetPostureCrumbleAttribute())
		Output = "Posture Crumble";
	else if (Attribute == USoulAttributeSet::GetAttackSpeedAttribute())
		Output = "Attack Speed";
	else if (Attribute == USoulAttributeSet::GetCriticalStrikeAttribute())
		Output = "Critical Strike";
	else if (Attribute == USoulAttributeSet::GetCriticalMultiAttribute())
		Output = "Critical Multiplier";
}
