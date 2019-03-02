// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionSysManager.generated.h"

class UGameplayAbility;
class USoulGameplayAbility;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SOUL_LIKE_ACT_API UActionSysManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionSysManager();

	class ASoul_Like_ACTCharacter *PlayerRef;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimCombo)
		TSubclassOf<UGameplayAbility> AttackGA;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	bool ActivateAbilitiesWithWeapon(bool bAllowRemoteActivation) const;

	void GetActiveAbilitiesWithTags(struct FGameplayTagContainer AbilityTags, TArray<USoulGameplayAbility*>& ActiveAbilities);

	bool JumpSectionForCombo() const;

public:
	bool bBlockMovement;
	bool bIsUsingMelee() const;
	bool bIsUsingAbility() const;
	bool bCanUseAnyGA() const;

	bool DoMeleeAttack();
};