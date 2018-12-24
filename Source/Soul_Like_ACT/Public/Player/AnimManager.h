// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AnimManager.generated.h"

UENUM(BlueprintType)
enum class EAttackQueueStatus : uint8
{
	Disabled,
	WaitForQueue,
	Queued,
	ManualAttackOnly
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API UAnimManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAnimManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	bool bIsDashing;
	bool bIsAttacking;

	bool bIsInCombat;

	bool bIsHit;

	bool bCanParry;

	EAttackQueueStatus AutoAttackQueue;

	int32 ChannelingPoints;

	int32 MaxComboCount;

	FTimerHandle ChannelingPointsTH;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<UAnimMontage*> ComboMontages;

	uint8 ComboIndex;

	void TryPlayComboMontage();

	void ResetComboIndex() { ComboIndex = 0; }

	void IncreaseComboIndex();

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
		EAttackQueueStatus GetAttackQueue() const { return AutoAttackQueue; }
	UFUNCTION(BlueprintCallable)
		void SetAttackQueue(EAttackQueueStatus AttackQueueStatus) { AutoAttackQueue = AttackQueueStatus; }
	UFUNCTION(BlueprintCallable)
		void EnableAttackQueue_InAnim() { AutoAttackQueue = EAttackQueueStatus::WaitForQueue; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetIsInCombat() const { return bIsInCombat; }
	UFUNCTION(BlueprintCallable)
		void SwitchModeBetweenCombatAndTravel() { bIsInCombat = !bIsInCombat; }

	UFUNCTION(BlueprintCallable)
		void ResetCombo();

	UFUNCTION(BlueprintCallable)
		void CancelAttacking()
	{
		bIsDashing = bIsAttacking = 0;
		ResetCombo();
	}

	UFUNCTION(BlueprintCallable)
		void SetCanParry(bool CanParry) { bCanParry = CanParry; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetCanParry() const { return bCanParry; }

	UFUNCTION(BlueprintCallable)
		void IncreasingChannelingPoints();
	UFUNCTION(BlueprintCallable)
		void ResetChannelingPoints() { ChannelingPoints = 0; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetChannelingPoints() const { return ChannelingPoints; }

	UFUNCTION(BlueprintCallable)
		void EnableIsAttacking() { bIsAttacking = 1; }
	UFUNCTION(BlueprintCallable)
		void DisableIsAttacking() { bIsAttacking = 0; }

	UFUNCTION(BlueprintCallable, Category = "Stuff")
		void TryAttack(bool bTriggeredByAnimBP, FString &DebugMessage);

	UFUNCTION(BlueprintCallable)
		static FString GetQueueStatusMessage(EAttackQueueStatus const &Inp);
	
};
