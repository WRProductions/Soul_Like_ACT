// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponActor.generated.h"

UCLASS()
class SOUL_LIKE_ACT_API AWeaponActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = 1))
		class USkeletalMeshComponent *MeshComp;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


protected:
	bool bIsTracingCollision;

	FTimerHandle SwingHandle, TracingHandle;

	UPROPERTY(BlueprintReadWrite)
		float BladeStartLength = 20.f;
	UPROPERTY(BlueprintReadWrite)
		float BladeTail = 120.f;

	TArray<FVector>PrevVecs;
	TArray<FVector>CurrVecs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<AActor*> MyTargets;

	void CheckCollision();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void DrawTraceLine(FVector prevVec_, FVector currVec_, bool bDrawTraceLine);

	bool TryExcludeActor(AActor *HitActor);

public:
	UFUNCTION(BlueprintCallable)
		void StartSwing();
	UFUNCTION(BlueprintCallable)
		void EndSwing();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		USoundBase *HitSound;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		UParticleSystem *BloodSplash;
};
