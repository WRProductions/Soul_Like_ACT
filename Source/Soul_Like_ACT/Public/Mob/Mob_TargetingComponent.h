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

	AActor *PlayerRef;

public:	
	// Sets default values for this component's properties
	UMob_TargetingComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	class AMobBasic *OwnerRef;
	class AMobController *OwnerController;

	bool bIsTargetingOn;

public:	
	UFUNCTION(BlueprintCallable)
		void ToggleTargetLocking();
};
