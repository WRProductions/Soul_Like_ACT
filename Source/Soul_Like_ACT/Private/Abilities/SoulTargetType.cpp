// Fill out your copyright notice in the Description page of Project Settings.

#include "Abilities/SoulTargetType.h"
#include "Abilities/SoulGameplayAbility.h"
#include "SoulCharacterBase.h"

void USoulTargetType::GetTargets_Implementation(ASoulCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	return;
}

void USoulTargetType_UseOwner::GetTargets_Implementation(ASoulCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	OutActors.Add(TargetingCharacter);
}

void USoulTargetType_UseEventData::GetTargets_Implementation(ASoulCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	const FHitResult* FoundHitResult = EventData.ContextHandle.GetHitResult();
	if (FoundHitResult)
	{
		OutHitResults.Add(*FoundHitResult);
	}
	else if (EventData.Target)
	{
		OutActors.Add(const_cast<AActor*>(EventData.Target));
	}
}

void USoulTargetType_LineTraceFront::GetTargets_Implementation(ASoulCharacterBase* TargetingCharacter, AActor* TargetingActor, FGameplayEventData EventData, TArray<FHitResult>& OutHitResults, TArray<AActor*>& OutActors) const
{
	UCapsuleComponent *TargetingCapsul = TargetingCharacter->GetCapsuleComponent();
	FVector InitLocation = TargetingCapsul->GetComponentLocation() + TargetingCapsul->GetComponentRotation().GetComponentForAxis(EAxis::X) * 70.f;
	FVector EndLocation = InitLocation + TargetingCapsul->GetComponentRotation().GetComponentForAxis(EAxis::X) * 140.f;

	TArray<FHitResult> Hits;
	UKismetSystemLibrary::LineTraceMultiForObjects(TargetingCharacter->GetWorld(), InitLocation, EndLocation
		, TArray<TEnumAsByte<EObjectTypeQuery>>{EObjectTypeQuery::ObjectTypeQuery3}
		, false
		, TArray<AActor*>{TargetingCharacter}
		, EDrawDebugTrace::ForDuration
		, Hits
		, true);

	OutHitResults = Hits;

	for (auto hit : Hits)
	{
		OutActors.Add(hit.GetActor());
	}
}
