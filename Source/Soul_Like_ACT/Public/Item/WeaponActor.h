// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Types/DA_Gear.h"
#include "WeaponActor.generated.h"

class ASoulCharacterBase;

UENUM(BlueprintType)
enum class EMeleeTraceType :  uint8
{
	Line,
	Capsule,
};

UCLASS()
class SOUL_LIKE_ACT_API AWeaponActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = 1))
	class USkeletalMeshComponent *MeshComp;

	ASoulCharacterBase *OwnerRef;

public:	
	// Sets default values for this actor's properties
	AWeaponActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GearInfo)
		UDA_Gear *GearInfo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = 1))
		bool bCanDamageAllies;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bIsTracingCollision;


	TArray<FVector>PrevVecs;
	TArray<FVector>CurrVecs;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	TArray<AActor*> MyTargets;

	EMeleeTraceType TraceType;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float DmgMultiplier;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	void CheckCollision();

	bool TryExcludeActor(AActor *HitActor);

	//real collision detection. Apply Damage through here
	void DrawTrace(FVector prevVec_, FVector currVec_, bool bDrawTraceLine);

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, meta = (ExposeOnSpawn = 1))
	bool bEnableDrawTraceLine;

	UFUNCTION(BlueprintCallable)
	void StartSwing(EMeleeTraceType MeleeTraceType, const float InDmgMulti = 1.f, const float InAreaMulti = 1.f);
	UFUNCTION(BlueprintCallable)
	void EndSwing();

	bool GetIsSwinging() const { return bIsTracingCollision; }

	UFUNCTION(BlueprintCallable)
	float GetDamageMagnitude() const { return DmgMultiplier; }

protected:
	UFUNCTION(BlueprintImplementableEvent)
	void ApplyEventBackToGA(ASoulCharacterBase *Target, const FHitResult &InpHitResult);
};