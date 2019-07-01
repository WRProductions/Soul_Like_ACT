// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MobActionManager.generated.h"

class USoulActiveAbility;
class AMobBasic;

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
	UMobActionManager();

	UFUNCTION(BlueprintCallable)
	void EnableComboQuery() { bNextComboInQuery = true; }
	UFUNCTION(BlueprintCallable)
	void InterruptComboQuery() { bNextComboInQuery = false; }

	UFUNCTION(BlueprintCallable)
	bool TryUseActiveAbility(TSubclassOf<USoulActiveAbility> InActiveAbility, const int32 InMontageIndex);

	UFUNCTION(BlueprintCallable)
	bool TryUseCombo(TSubclassOf<USoulActiveAbility> InActiveAbility, bool bForceNew);

	friend class AMobBasic;
};
