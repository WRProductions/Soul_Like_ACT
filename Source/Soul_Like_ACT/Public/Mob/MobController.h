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

protected:
	class AMobBasic *PossessedMob;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = AI)
		class UAIPerceptionComponent* AIPerceptionComponent;
	
	UAISenseConfig_Sight *sightConfig;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = AI)
		class UBlackboardComponent *BlockBoardComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = AI)
		class UBehaviorTreeComponent *BehaviorTreeComp;

public:
	AMobController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIAssets)
		class UBlackboardData *BlockBoardData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AIAssets)
		class UBehaviorTree *BehaviorTreeAsset;

protected:
	virtual void BeginPlay() override;

public:
	/**
	* AI usages
	*/
	UFUNCTION(BlueprintCallable)
	void GainRageOnHit(AActor* SourceActor, AActor* TargetActor, const FHitResult HitResult);

	UFUNCTION(BlueprintCallable)
	void AISenseUpdateMessage(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetAIEnabled(bool Enable);

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	virtual void Tick(float DeltaTime) override;

	class UBehaviorTreeComponent *GetBTComp() const { return BehaviorTreeComp; }
};