// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/AnimManager.h"
#include "TimerManager.h"
#include "GameFramework/Character.h"


// Sets default values for this component's properties
UAnimManager::UAnimManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UAnimManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UAnimManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAnimManager::TryPlayComboMontage()
{
	Cast<ACharacter>(GetOwner())->PlayAnimMontage(ComboMontages[ComboIndex], 1.f);
	IncreaseComboIndex();
}

void UAnimManager::IncreaseComboIndex()
{
	++ComboIndex;

	if (ComboIndex >= ComboMontages.Num())
	{
		ResetComboIndex();
	}
}

void UAnimManager::ResetCombo()
{
	SetAttackQueue(EAttackQueueStatus::Disabled);

	ResetComboIndex();
}

void UAnimManager::IncreasingChannelingPoints()
{
	++ChannelingPoints;
	//GetWorld()->GetTimerManager().SetTimer(ChannelingPointsTH, this, &UInputBuffComp::ResetChannelingPoints, .5f, 0, 3.f);
}

void UAnimManager::TryAttack(bool bTriggeredByAnimBP, FString & DebugMessage)
{
	if (!bIsInCombat)
	{
		DebugMessage = "Not In Combat";
		return;
	}
	else if (bIsAttacking)
	{
		if (AutoAttackQueue == EAttackQueueStatus::Disabled)
		{
			DebugMessage = "Is Attacking, Cant take input";
			return;
		}
		else if (AutoAttackQueue == EAttackQueueStatus::WaitForQueue)
		{
			if (bTriggeredByAnimBP)
			{
				SetAttackQueue(EAttackQueueStatus::ManualAttackOnly);
				DebugMessage = "Waiting for manual input";
				return;
			}
			//Triggered Manually
			else
			{
				SetAttackQueue(EAttackQueueStatus::Queued);
				DebugMessage = "Queued";
				return;
			}
		}
		else if (AutoAttackQueue == EAttackQueueStatus::Queued)
		{
			if (bTriggeredByAnimBP)
			{
				SetAttackQueue(EAttackQueueStatus::Disabled);
				TryPlayComboMontage();
				DebugMessage = "Attacking";
				return;
			}
			else
			{
				DebugMessage = "Queued, Cant take input";
			}
		}
		else if (AutoAttackQueue == EAttackQueueStatus::ManualAttackOnly
			&& !bTriggeredByAnimBP)
		{
			SetAttackQueue(EAttackQueueStatus::Disabled);
			TryPlayComboMontage();
			DebugMessage = "Manual trigger attacking";
			return;
		}
	}
	else if (
		AutoAttackQueue == EAttackQueueStatus::Disabled
		&& !bTriggeredByAnimBP)
	{
		SetAttackQueue(EAttackQueueStatus::Disabled);
		TryPlayComboMontage();
		DebugMessage = "First attacking";
		return;
	}
	DebugMessage = "Do Nothing";
	return;
}

FString UAnimManager::GetQueueStatusMessage(EAttackQueueStatus const & Inp)
{
	switch (Inp)
	{
	case EAttackQueueStatus::Disabled:
		return "EAttackQueueStatus::Disabled";
	case EAttackQueueStatus::ManualAttackOnly:
		return "EAttackQueueStatus::ManualAttackOnly";
	case EAttackQueueStatus::Queued:
		return "EAttackQueueStatus::Queued";
	case EAttackQueueStatus::WaitForQueue:
		return "EAttackQueueStatus::WaitForQueue";
	default:
		return "Invalid Input";
	}
}

