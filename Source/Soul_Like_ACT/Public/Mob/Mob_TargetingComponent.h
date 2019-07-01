// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mob_TargetingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SOUL_LIKE_ACT_API UMob_TargetingComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	bool bIsLockingOn;

public:	
	// Sets default values for this component's properties
	UMob_TargetingComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	class AMobBasic *OwnerRef;

	AActor *TargetPawn;
	bool bIsFacingTarget;
	bool bFreeRotation;

	float MoveSpeedCoe = 1.0f;

	void SetMoveSpeedCoe(bool bEnabled);

public:
	//Whether the rotation can interp to facing the target
	UFUNCTION(BlueprintCallable)
	void SetFreeRotation(bool bEnabled);

	void FacingTarget_Init(AActor *TargetActor);
	void FacingTarget_End();

	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor *PlayerPawn) { TargetPawn = PlayerPawn; }

	UFUNCTION(BlueprintCallable)
	bool GetIsFacingTarget() const{ return bIsFacingTarget; }
	UFUNCTION(BlueprintCallable)
	bool GetIsFreeRotation() const { return bFreeRotation; }

	friend class AMobBasic;
};