// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MobActionManager.generated.h"

class USoulActiveAbility;
class AMobBasic;

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SOUL_LIKE_ACT_API UMobActionManager : public UActorComponent
{
	GENERATED_BODY()

protected:
	AMobBasic* OwnerRef;

	bool bNextComboInQuery;

	uint8 CurrComboStage;
public:
	UMobActionManager();

	UFUNCTION(BlueprintCallable)
	void EnableComboQuery() 
	{
		bNextComboInQuery = true;
		CurrComboStage = 0;
	}

	//Call that to prevent the combo being triggered.
	UFUNCTION(BlueprintCallable)
		void ResetComboQuery() { bNextComboInQuery = false; CurrComboStage = 0; }

	UFUNCTION(BlueprintCallable)
		const bool GetIsComboInQuery() const { return bNextComboInQuery; }

	UFUNCTION(BlueprintCallable)
		bool TryUseActiveAbility(TSubclassOf<USoulActiveAbility> InActiveAbility, bool bUseCombo = false);

	UFUNCTION(BlueprintCallable)
		bool UseNextCombo(class UAnimInstance *AnimInstance);

	UFUNCTION(BlueprintCallable)
		static UMobActionManager* GetMobActionManager(AActor* SourceActor);
	friend class AMobBasic;
};