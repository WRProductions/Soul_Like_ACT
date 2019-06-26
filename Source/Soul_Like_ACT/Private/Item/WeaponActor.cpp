// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/WeaponActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Abilities/SoulAbilitySysBPLib.h"
#include "SoulCharacterBase.h"
#include "DrawDebugHelpers.h"

// Sets default values
AWeaponActor::AWeaponActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComp");
	MeshComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();

	OwnerRef = Cast<ASoulCharacterBase>(GetInstigator());
	check(OwnerRef);

	check(GearInfo);
}

void AWeaponActor::CheckCollision()
{
	PrevVecs = CurrVecs;
	for (int8 i = 0; i < PrevVecs.Num(); ++i)
	{
		if (TraceType == EMeleeTraceType::Line)
			CurrVecs[i] = GetActorLocation() + i * GetActorUpVector() * 25.f;
		else if (TraceType == EMeleeTraceType::Capsule)
			CurrVecs[i] = GetActorLocation() + i * GetActorUpVector() * 50.f;

		DrawTrace(PrevVecs[i], CurrVecs[i], bEnableDrawTraceLine);
	}
}

// Exclude the actor which will receive the damage
// Return true if the Actor has not been excluded yet
bool AWeaponActor::TryExcludeActor(AActor * HitActor)
{
	if (MyTargets.Contains(HitActor))
	{
		return 0;
	}
	MyTargets.Add(HitActor);
	return 1;
}

void AWeaponActor::DrawTrace(FVector prevVec_, FVector currVec_, bool bDrawTraceLine)
{
	TArray<FHitResult> Hits;
 	FCollisionQueryParams QueryParams;
 	QueryParams.AddIgnoredActor(this);
 	QueryParams.AddIgnoredActor(OwnerRef);
 	QueryParams.bTraceComplex = false;
 	QueryParams.TraceTag = FName("WeaponMelee");

	if (bDrawTraceLine)
		GetWorld()->DebugDrawTraceTag = FName("WeaponMelee");
	else
		GetWorld()->DebugDrawTraceTag = NAME_None;

	bool bIsHit = false;
	if (TraceType == EMeleeTraceType::Line)
	{
		bIsHit = GetWorld()->LineTraceMultiByChannel(Hits, prevVec_, currVec_, ECC_Pawn, QueryParams);
	}
	else if (TraceType == EMeleeTraceType::Capsule)
	{
		FCollisionShape CollisionShape;
		CollisionShape.SetCapsule(30.f, 30.f);
		bIsHit = GetWorld()->SweepMultiByChannel(Hits, prevVec_, currVec_, FQuat::Identity, ECC_Pawn, CollisionShape, QueryParams);
	}
	if (bIsHit)
	{
		for (const auto& Hit : Hits)
		{
			ASoulCharacterBase* TargetPawn = Cast<ASoulCharacterBase>(Hit.GetActor());
			if (TargetPawn
				&& ASoulCharacterBase::IsInRivalFaction(OwnerRef, TargetPawn)
				&& TryExcludeActor(TargetPawn))
			{
				ApplyEventBackToGA(TargetPawn, Hit);
			}
		}
	}
}

void AWeaponActor::StartSwing(EMeleeTraceType MeleeTraceType, const float InDmgMulti /*= 1.f*/, const float InAreaMulti /*= 100.f*/)
{
	bIsTracingCollision = 1;

	DmgMultiplier = InDmgMulti;
	TraceType = MeleeTraceType;

	CurrVecs.Reset();
	MyTargets.Empty();

	if (GearInfo->SwingSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), GearInfo->SwingSound, GetActorLocation());

	if (TraceType == EMeleeTraceType::Line)
	{
		int32 i = GearInfo->BladeStartLength;
		CurrVecs.Add(GetActorLocation() + i * GetActorUpVector());

		do
		{
			i += 25;
			CurrVecs.Add(GetActorLocation() + i * GetActorUpVector());
		} while (i <= GearInfo->BladeTail * 0.01f * InAreaMulti);
	}
	else if (TraceType == EMeleeTraceType::Capsule)
	{
		int32 i = GearInfo->BladeStartLength;
		CurrVecs.Add(GetActorLocation() + i * GetActorUpVector());

		do
		{
			i += 60;
			CurrVecs.Add(GetActorLocation() + i * GetActorUpVector());
		} while (i <= GearInfo->BladeTail * 0.01f * InAreaMulti);
	}
}

void AWeaponActor::EndSwing()
{
	bIsTracingCollision = 0;
}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsTracingCollision)
		CheckCollision();
}