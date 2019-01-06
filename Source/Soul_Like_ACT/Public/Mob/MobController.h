// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig.h"
#include "MobController.generated.h"

/**
 * 
 */
UCLASS()
class SOUL_LIKE_ACT_API AMobController : public AAIController
{
	GENERATED_BODY()

	class AMobBasic *PossessedMob;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = AI)
		UAISenseConfig_Sight *sightConfig;

public:
	AMobController();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable)
	void AISenseUpdateMessage(AActor* Actor, FAIStimulus Stimulus);

	virtual void Possess(APawn* InPawn) override;
	virtual void UnPossess() override;

	virtual void Tick(float DeltaTime) override;
};