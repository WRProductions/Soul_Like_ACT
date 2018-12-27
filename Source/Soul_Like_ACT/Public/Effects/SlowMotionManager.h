// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlowMotionManager.generated.h"

UCLASS()
class SOUL_LIKE_ACT_API ASlowMotionManager : public AActor
{
	GENERATED_BODY()

	static FTimerHandle SlowMoHandle;

	static void CountDownSlowMo();

public:	
	// Sets default values for this actor's properties
	ASlowMotionManager();


	UFUNCTION(BlueprintCallable)
		static void InitSlowMotion(AActor *Attacker, AActor *Receiver, float HitStrength = 1.f);

};
