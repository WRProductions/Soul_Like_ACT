// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

UCLASS()
class SOUL_LIKE_ACT_API AWeaponActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = 1))
	class USkeletalMeshComponent *MeshComp;

	class ATargetableActor *OwnerRef;

public:	
	// Sets default values for this actor's properties
	AWeaponActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = 1))
		bool bCanDamageAllies;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = 1))
		float WeaponDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USoundBase *SwingSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem *BladeCollisionFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystem *OnHitFX;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bIsTracingCollision;

	FTimerHandle SwingHandle, TracingHandle;

	UPROPERTY(BlueprintReadWrite)
	int32 BladeStartLength = 20;
	UPROPERTY(BlueprintReadWrite)
	int32 BladeTail = 120;

	TArray<FVector>PrevVecs;
	TArray<FVector>CurrVecs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AActor*> MyTargets;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	void CheckCollision();

	bool TryExcludeActor(AActor *HitActor);

	void DrawTraceLine(FVector prevVec_, FVector currVec_, bool bDrawTraceLine);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ShowDrawTraceLine, meta = (ExposeOnSpawn = 1))
		bool bEnableDrawTraceLine;

	UFUNCTION(BlueprintCallable)
		void StartSwing();
	UFUNCTION(BlueprintCallable)
		void EndSwing();

	bool GetIsSwinging() const { return bIsTracingCollision; }
};
