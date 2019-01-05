// Fill out your copyright notice in the Description page of Project Settings.

#include "Item/WeaponActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TargetableActor.h"
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

	OwnerRef = Cast<ATargetableActor>(GetInstigator());
	check(OwnerRef);
}

void AWeaponActor::CheckCollision()
{
	PrevVecs = CurrVecs;
	for (int i = BladeStartLength; i <= BladeTail; i += 20)
	{
		int32 size_ = (i - BladeStartLength) / 10;

		CurrVecs[size_] = GetActorLocation() + i * GetActorUpVector();

		DrawTraceLine(PrevVecs[size_], CurrVecs[size_], bEnableDrawTraceLine);
	}
}

void AWeaponActor::DrawTraceLine_Implementation(FVector prevVec_, FVector currVec_, bool bDrawTraceLine)
{
	TArray<FHitResult> Hits;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(OwnerRef);

	if (bDrawTraceLine)
		DrawDebugLine(GetWorld(), prevVec_, currVec_, FColor::Green, 0, 2.f, 0, 1.f);

	bool bIsHit = GetWorld()->LineTraceMultiByChannel(Hits, prevVec_, currVec_, ECC_Pawn, QueryParams);
	if (bIsHit)
	{
		for (const auto &Hit : Hits)
		{
			ATargetableActor * tempChar = Cast<ATargetableActor>(Hit.GetActor());
			if (tempChar
				&& ATargetableActor::IsInRivalFaction(OwnerRef, tempChar) 
				&& TryExcludeActor(Hit.GetActor()))
			{
				UE_LOG(LogTemp, Warning, TEXT("%s"), *Hit.GetActor()->GetName());
				
				OnSlowMotionTrigger.Broadcast(1.f, tempChar);
				
				UGameplayStatics::ApplyDamage(Hit.GetActor(), 30.f, GetInstigatorController(), GetOwner(), UDamageType::StaticClass());
				if (HitSound)
					UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, Hit.Location);
				if (BloodSplash)
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodSplash, Hit.ImpactPoint, FRotator::ZeroRotator, 1);;
			}
		}
	}
	else
	{
		if (bDrawTraceLine) 
			DrawDebugLine(GetWorld(), prevVec_, currVec_, FColor::Red, 0, 2.f, 0, 1.f);
	}
}

bool AWeaponActor::TryExcludeActor(AActor * HitActor)
{
	if (MyTargets.Contains(HitActor))
	{
		return 0;
	}
	MyTargets.Add(HitActor);
	return 1;
}

void AWeaponActor::StartSwing()
{
	bIsTracingCollision = 1;

	for (int i = BladeStartLength; i <= BladeTail; i += 10)
	{
		CurrVecs.Add(GetActorLocation() + i * GetActorUpVector());
	}
}

void AWeaponActor::EndSwing()
{
	bIsTracingCollision = 0;
	CurrVecs.Reset();
	MyTargets.Empty();
}

// Called every frame
void AWeaponActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsTracingCollision)
		CheckCollision();
}

