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
	ManualAttackOnly
};

UENUM(BlueprintType)
enum class EActionType : uint8
{
	Attack,
	Dodge,
	Parry,
	Block,
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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsStun;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsActing;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsBlocking;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsParry;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bIsDodging;

protected:

	EAttackQueueStatus AutoAttackQueue;
	EActionType MyActionType;

	int32 ChannelingPoints;

	int32 MaxComboCount;

	FTimerHandle Handle_WaitToParry;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimMontages)
		TArray<UAnimMontage*> ComboMontages;
	uint8 ComboIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimMontages)
		UAnimMontage *Dash_Forward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimMontages)
		UAnimMontage *BlockMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AnimMontages)
		UAnimMontage *ParryMontage;

	bool bCanParry;
	//Delay Time: 0.2s
	void Timer_WaitToParry();

	void PlayMontage();
	void PlayComboMontage();
	void PlayDodgeMontage();

	void PlayParryMontage_Pressed();

	void ResetComboIndex() { ComboIndex = 0; }

	void IncreaseComboIndex();

	void ResetParryStatus() { bIsBlocking = bIsParry = 0; }

public:


	//Call this every time being hit or use animations other than attacking
	UFUNCTION(BlueprintCallable)
		void ResetCombo();

	void PlayParryMontage_Released();

	/*
	UFUNCTION(BlueprintCallable)
		void SetIsBlocking(bool IsBlocking) { bIsBlocking = IsBlocking; }
	UFUNCTION(BlueprintCallable)
		void SetIsParry(bool IsParry) { bIsParry = IsParry; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetIsParry() const { return bIsParry; }
	UFUNCTION(BlueprintCallable, BlueprintPure)
		bool GetIsBlocking() const { return bIsBlocking; }
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure)
		EAttackQueueStatus GetAttackQueue() const { return AutoAttackQueue; }
	UFUNCTION(BlueprintCallable)
		void SetAttackQueue(EAttackQueueStatus AttackQueueStatus) { AutoAttackQueue = AttackQueueStatus; }

	UFUNCTION(BlueprintCallable)
		void EnableAttackQueue_InAnim() { AutoAttackQueue = EAttackQueueStatus::WaitForQueue; }

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
		void EnableActing();
	UFUNCTION(BlueprintCallable)
		void DisableActing();
	
	UFUNCTION(BlueprintCallable)
		bool GetCanMove() const { return !bIsActing; }

	UFUNCTION(BlueprintCallable)
		void TryUseDequeMotion(bool bTriggeredByAnimBP, FString &DebugMessage);

	UFUNCTION(BlueprintCallable)
		void SetActionType(const EActionType InpActionType) { MyActionType = InpActionType; }

	void TryUseDequeMotion(const EActionType InpActionType, bool bTriggeredByAnimBP, FString &DebugMessage);

	UFUNCTION(BlueprintCallable)
		static FString GetQueueStatusMessage(EAttackQueueStatus const &Inp);

	UFUNCTION(BlueprintCallable)
		void SetIsStun(bool IsStun);
	
	UPROPERTY(BlueprintAssignable)
		FOnStun OnStun_Delegate;
};
