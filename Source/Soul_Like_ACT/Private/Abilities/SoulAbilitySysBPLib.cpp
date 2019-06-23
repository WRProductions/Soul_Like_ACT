// Fill out your copyright notice in the Description page of Project Settings.

#include "SoulAbilitySysBPLib.h"
#include "Abilities/SoulGameplayAbility.h"
#include "Animation/AnimMontage.h"

USoulAbilitySysBPLib::USoulAbilitySysBPLib(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void USoulAbilitySysBPLib::CreateEventData(const AActor *Target, const AActor *Source, const FHitResult &InpHitResult, const FGameplayTag EventTag , const float EventMagnitude, FGameplayEventData &OutpEventData)
{
	FGameplayEffectContextHandle TempContextHandle(new FGameplayEffectContext());
	TempContextHandle.AddHitResult(InpHitResult);

	FGameplayEventData TempEventData;
	TempEventData.Instigator = Source;
	TempEventData.Target = Target;
	//TODO damage magnitude
	TempEventData.EventMagnitude = EventMagnitude;
	TempEventData.EventTag = EventTag;
	TempEventData.ContextHandle = TempContextHandle;

	OutpEventData = TempEventData;
}

bool USoulAbilitySysBPLib::GetMontageFromActiveAbility(TSubclassOf<USoulActiveAbility> ActiveAbilityClass, UAnimMontage*& AnimMontage)
{
	 USoulActiveAbility *LocalAbility = Cast<USoulActiveAbility>(ActiveAbilityClass->GetDefaultObject());
	 if (LocalAbility)
	 {
		 AnimMontage = LocalAbility->MontageToPlay;
		 return AnimMontage != nullptr;
	 }

	 AnimMontage = NULL;
	 return false;
}

float USoulAbilitySysBPLib::GetMontageSectionLength(UAnimMontage* AnimMontage, FName SectionName)
{
	int32 SectionIndex = AnimMontage->GetSectionIndex(SectionName);
	
	if (SectionIndex == INDEX_NONE)
		return -1.f;

	return AnimMontage->GetSectionLength(SectionIndex);
}
