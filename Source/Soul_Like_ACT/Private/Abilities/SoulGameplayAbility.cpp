// Fill out your copyright notice in the Description page of Project Settings.

#include "SoulGameplayAbility.h"

FSoulGameplayEffectContainerSpec USoulGameplayAbility::MakeEffectContainerSpecFromContainer(const FSoulGameplayEffectContainer & Container, const FGameplayEventData & EventData, int32 OverrideGameplayLevel)
{
	return FSoulGameplayEffectContainerSpec();
}

FSoulGameplayEffectContainerSpec USoulGameplayAbility::MakeEffectContainerSpec(FGameplayTag ContainerTag, const FGameplayEventData & EventData, int32 OverrideGameplayLevel)
{
	return FSoulGameplayEffectContainerSpec();
}

TArray<FActiveGameplayEffectHandle> USoulGameplayAbility::ApplyEffectContainerSpec(const FSoulGameplayEffectContainerSpec & ContainerSpec)
{
	return TArray<FActiveGameplayEffectHandle>();
}

TArray<FActiveGameplayEffectHandle> USoulGameplayAbility::ApplyEffectContainer(FGameplayTag ContainerTag, const FGameplayEventData & EventData, int32 OverrideGameplayLevel)
{
	return TArray<FActiveGameplayEffectHandle>();
}
