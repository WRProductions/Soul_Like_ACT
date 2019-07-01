// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MobActionManager.generated.h"

class USoulActiveAbility;
class AMobBasic;

UENUM(BlueprintType)
enum class EMobActionState : uint8
{
	Idel,
	Attack,
	Dodge,
	Parry,
	Block,
	OnHit,
	OnStun,
	OnDead,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API UMobActionManager : public UActorComponent
{
	GENERATED_BODY()

protected:
	AMobBasic* OwnerRef;

	bool bNextComboInQuery;

	uint8 CurrComboStage;
	TSubclassOf<USoulActiveAbility> CurrActiveAbility;

public:	
	// Sets default values for this component's properties
	UMobActionManager();

	EMobActionState MobActionState;

	void EnableComboQuery() { bNextComboInQuery = true; }

	void RenewComboStatge(TSubclassOf<USoulActiveAbility> InActiveAbility, const bool bNewGA);

	bool TryUseActiveAbility(TSubclassOf<USoulActiveAbility> InActiveAbility, const float InMultiplier, const int32 InMontageIndex);

	bool TryUseCombo(TSubclassOf<USoulActiveAbility> InActiveAbility, const float InMultiplier, bool bForceNew);

	friend class AMobBasic;
};
