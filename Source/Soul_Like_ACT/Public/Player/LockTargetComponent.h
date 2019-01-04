// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockTargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API ULockTargetComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	FTimerHandle TargetBlockingHandler;

	class AActor *SelectedActor;

	bool bIsTargetingEnabled;

	bool bFreeCamera;

	bool bOwnerControllerRotationYaw = 0
		, bOwnerOrientRotToMovement = 1
		, bOwnerControllerDesiredRot = 0;

	TArray<AActor*> PotentialTargetActors;

	class UArrowComponent *PlayerArrow;
	class ACharacter *PlayerRef;

public:	
	// Sets default values for this component's properties
	ULockTargetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Detection Stages-----------
	void FindTarget();
	
	void GetPotentialTargetsInScreen(TArray<AActor *> &OutPotentialTargets);
	void RuleOutBlockedTargets(TArray<AActor *> LocalPotentialTargets, TArray<AActor *> &OutPotentialTargets);
	void FindClosestTargetInScreen(TArray<AActor *> LocalPotentialTargets, AActor *&ClosestTarget);
	//---------------------------

	void EnableLockingTarget();
	void DisableLockingTarget();

	void CacheRotationSetting();
	void ResetRotationSetting();

	void SetRotationMode_FaceTarget();

	bool IsTraceBlocked(AActor *SelectedTarget, TArray<AActor*> IgnoredActors, const ECollisionChannel TraceChannel);
	FVector GetLineTraceStartLocation();

	void Timer_CheckBlockingAndDistance();

	void Tick_UpdateRotation();
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
		void ToggleCameraLock(bool FreeCamera);

	void InitComponent(class UArrowComponent *ArrowComponentRef);

	bool GetIsTargetingEnabled() { return bIsTargetingEnabled; }
};
