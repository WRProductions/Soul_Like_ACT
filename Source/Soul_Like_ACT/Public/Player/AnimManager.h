// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AnimManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStun, bool, IsStun);

UENUM(BlueprintType)
enum class EAttackQueueStatus : uint8
{
	Disabled,
	WaitForQueue,
	Queued,
	ManualInputOnly
};

UENUM(BlueprintType)
enum class EInputState : uint8
{
	Loco,
	Attack_Pre,
	Attack_Light,
	Attack_Heavy,
	Dodge,
	Parry,
	Block,
	OnHit,
	OnStun,
	OnDead,
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API UAnimManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAnimManager();

	class ASoul_Like_ACTCharacter *PlayerRef;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	EInputState InputState;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bStopMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UDA_PlayerAnimSet *DefaultAnimSet;

	UPROPERTY(VisibleAnywhere)
	class UDA_AttackMontage *CurrentComboStage;

protected:
	EAttackQueueStatus AutoAttackQueue;

	int32 ChannelingPoints;

	int32 MaxComboCount;

	FTimerHandle Handle_WaitToParry;
	bool bCanCastParry;
	void Timer_ResetCanTriggerParry();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimMontages)
		UAnimMontage *Dash_Forward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimMontages)
		UAnimMontage *BlockMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimMontages)
		UAnimMontage *ParryMontage;

	void PlayMontage();
	void PlayPreAttack();
	void PlayLightAttack();
	void PlayHeavyAttack();
	void PlayDodgeMontage();
	void PlayBlockOrParry();

	void ResetComboIndex();
	void GetNextAttackDA();

	void ResetParryStatus() { bCanCastParry = 0; }

public:
	//Prevent movement input while montage is playing
	UFUNCTION(BlueprintCallable)
		void EnableActing();
	UFUNCTION(BlueprintCallable)
		void EndActing();

	//Call this every time being hit or use animations other than attacking
	UFUNCTION(BlueprintCallable)
		void ResetCombo();

	void PlayParryMontage_Released();

	UFUNCTION(BlueprintCallable, BlueprintPure)
		EAttackQueueStatus GetAttackQueue() const { return AutoAttackQueue; }
	
	void SetAttackQueue(EAttackQueueStatus AttackQueueStatus) { AutoAttackQueue = AttackQueueStatus; }

	UFUNCTION(BlueprintCallable)
		void EnableAttackQueue_InAnim() { AutoAttackQueue = EAttackQueueStatus::WaitForQueue; }
	UFUNCTION(BlueprintCallable)
		void DisableAttackQueue_InAnim() { AutoAttackQueue = EAttackQueueStatus::Disabled; }

	UFUNCTION(BlueprintCallable)
		void IncreasingChannelingPoints();
	UFUNCTION(BlueprintCallable)
		void ResetChannelingPoints() { ChannelingPoints = 0; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
		int32 GetChannelingPoints() const { return ChannelingPoints; }
	
	UFUNCTION(BlueprintCallable)
		bool GetCanMove() const { return !bStopMovement; }

	UFUNCTION(BlueprintCallable)
		void TryUseDequeMotion(bool bTriggeredByAnimBP, FString &DebugMessage);

	void TryUseDequeMotion(const EInputState InpActionType, bool bTriggeredByAnimBP, FString &DebugMessage);

	UFUNCTION(BlueprintCallable)
		static FString GetQueueStatusMessage(EAttackQueueStatus const &Inp);

	UFUNCTION(BlueprintCallable)
		void SetIsStun(bool IsStun);
};
