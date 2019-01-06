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
	class AMobController *OwnerController;

	AActor *TargetPawn;
	bool bIsFacingTarget;

	void FacingTarget_Start();
	void FacingTarget_End();

public:	
	//Toggled through AI BT
	UFUNCTION(BlueprintCallable)
	void ToggleTargetLocking();

	void SetTarget(AActor *PlayerPawn) 
	{ 
		TargetPawn = PlayerPawn;
	}
};