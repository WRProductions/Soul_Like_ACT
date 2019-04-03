// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/LockTargetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Interfaces/Targetable.h"
#include "DrawDebugHelpers.h"
#include "SoulCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Player/ActionSysManager.h"
#include "TimerManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values for this component's properties
ULockTargetComponent::ULockTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<ACharacter>(GetOwner());
	check(PlayerRef);

}


// Called every frame
void ULockTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Tick_UpdateRotation();
}


void ULockTargetComponent::ToggleCameraLock(bool FreeCamera)
{
	if (!bIsTargetingEnabled)
	{
		bFreeCamera = FreeCamera;
		FindTarget(ETargetFindingDirection::Centre);
	}
	else
	{
		DisableLockingTarget();
	}
}



void ULockTargetComponent::InitComponent(class UArrowComponent *ArrowComponentRef)
{
	ArrowComponentRef->bAbsoluteRotation = 1;
	PlayerArrow = ArrowComponentRef;

	check(PlayerArrow);

	PlayerArrow->SetVisibility(0);
}


bool ULockTargetComponent::SetForceFacingOffset(bool Inp)
{
	if (bIsTargetingEnabled)
		bForcedFacingOffset = Inp;
	
	return Inp;
}

void ULockTargetComponent::FindTarget(ETargetFindingDirection Direction /*= ETargetFindingDirection::Centre*/)
{
	TArray<AActor*> LocalPotentialTargets, LocalPotentialTargets_Stage_2;
	
	GetPotentialTargetsInScreen(LocalPotentialTargets);

	if (LocalPotentialTargets.Num() == 0) { return DisableLockingTarget(); }

	RuleOutBlockedTargets(LocalPotentialTargets, LocalPotentialTargets_Stage_2);

	if (LocalPotentialTargets_Stage_2.Num() == 0) { return DisableLockingTarget(); }

	if (Direction == ETargetFindingDirection::Centre)
		FindClosestTargetInScreen(LocalPotentialTargets_Stage_2, SelectedActor);
	else
		Find_InDirection(LocalPotentialTargets_Stage_2, SelectedActor, Direction);

	if (SelectedActor)
		EnableLockingTarget();
}


void ULockTargetComponent::GetPotentialTargetsInScreen(TArray<AActor *> &OutPotentialTargets)
{
	TArray<AActor*> TargetableActors;

	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UTargetable::StaticClass(), TargetableActors);

	for (auto *TargetableActor : TargetableActors)
	{
		if(TargetableActor == GetOwner()) continue;

		//Is the potential actor target-able
		auto *Targetable = Cast<ITargetable>(TargetableActor);
		if (!Targetable->IsTargetable() && TargetableActor != GetOwner()) continue;


		//Mesure distance between target and player pawn
		if (TargetableActor->GetDistanceTo(GetOwner()) >= 2500.f) continue;

		//Measure whether target in screen
		FVector2D TargetAtScreenPosition;
		if (!UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), TargetableActor->GetActorLocation(), TargetAtScreenPosition, 0))
			continue;

		OutPotentialTargets.Add(TargetableActor);
	}
}


void ULockTargetComponent::RuleOutBlockedTargets(TArray<AActor *> LocalPotentialTargets, TArray<AActor *> &OutPotentialTargets)
{
	for (auto *TargetableActor : LocalPotentialTargets)
	{
		if (!IsTraceBlocked(TargetableActor, TArray<AActor*>{TargetableActor}, ECC_Camera))
			OutPotentialTargets.Add(TargetableActor);
	}
}


void ULockTargetComponent::FindClosestTargetInScreen(TArray<AActor *> &LocalPotentialTargets, AActor *&ClosestTarget)
{
	int32 LocalX, LocalY;
	GetWorld()->GetFirstPlayerController()->GetViewportSize(LocalX, LocalY);
	FVector2D ScreenCentre{ LocalX * .5f, LocalY * .5f };

	//Get first potential target
	FVector2D TargetScreenPosition;

	UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), LocalPotentialTargets[0]->GetActorLocation(), TargetScreenPosition, 0);
	float ClosestScreenDistance = FVector2D::Distance(TargetScreenPosition, ScreenCentre);
	AActor *TempClosestTarget = LocalPotentialTargets[0];

	for (int i = 1; i < LocalPotentialTargets.Num(); ++i)
	{
		UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), LocalPotentialTargets[i]->GetActorLocation(), TargetScreenPosition, 0);
		float LocalScreenDistance = FVector2D::Distance(TargetScreenPosition, ScreenCentre);

		if (LocalScreenDistance < ClosestScreenDistance)
		{
			TempClosestTarget = LocalPotentialTargets[i];
			ClosestScreenDistance = LocalScreenDistance;
		}
	}

	if (TempClosestTarget)
	{
		ClosestTarget = TempClosestTarget;
		//UE_LOG(LogTemp, Warning, TEXT("Target Position on Screen: %s, Screen Centre Vec: %s"), *TargetScreenPosition.ToString(), *ScreenCentre.ToString());
	}
}


void ULockTargetComponent::Find_InDirection(TArray<AActor *> &LocalPotentialTargets, AActor *&ClosestTarget, ETargetFindingDirection Direction)
{
	if (!bIsTargetingEnabled || !SelectedActor)
	{
		return;
	}
	else
	{
		AActor *TempClosestTarget = nullptr;

		FVector2D SelectedVector;
		UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(),
			ClosestTarget->GetActorLocation(),
			SelectedVector);

		//Get first potential target
		FVector2D TargetScreenPosition;

		float ClosestScreenDistance = 100000.f;

		for (int i = 0; i < LocalPotentialTargets.Num(); ++i)
		{
			//Skip the selected actor
			if(SelectedActor == LocalPotentialTargets[i]) continue;

			UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(),
				LocalPotentialTargets[i]->GetActorLocation(),
				TargetScreenPosition);

			//Compare X
			if (Direction == ETargetFindingDirection::Left && TargetScreenPosition.X > SelectedVector.X) 
				continue;
			if (Direction == ETargetFindingDirection::Right && TargetScreenPosition.X < SelectedVector.X)
				continue;

			float LocalScreenDistance = FVector2D::Distance(TargetScreenPosition, SelectedVector);

			if (LocalScreenDistance < ClosestScreenDistance)
			{
				TempClosestTarget = LocalPotentialTargets[i];
				ClosestScreenDistance = LocalScreenDistance;
			}
		}

		if (TempClosestTarget)
		{
			Cast<ASoulCharacterBase>(ClosestTarget)->ToggleLockIcon(false);
			ClosestTarget = TempClosestTarget;
			//UE_LOG(LogTemp, Warning, TEXT("Target Position on Screen: %s, Screen Centre Vec: %s"), *TargetScreenPosition.ToString(), *ScreenCentre.ToString());
		}
	}
}


void ULockTargetComponent::EnableLockingTarget()
{
	bIsTargetingEnabled = 1;

	//Save Rotation references
	CacheRotationSetting();

	//Target->OnSelected
	Cast<ITargetable>(SelectedActor)->ToggleLockIcon(1);

	//Set Rotation Mode
	SetRotationMode_FaceTarget();

	//Handler: Detect whether the actor is still detectable
	GetWorld()->GetTimerManager().SetTimer(TargetBlockingHandler, this, &ULockTargetComponent::Timer_CheckBlockingAndDistance, .15f, 1, 0.f);

	//Ingnore look input
	GetOwner()->GetInstigatorController()->SetIgnoreLookInput(!bFreeCamera);

	//Set Arrow visible
	PlayerArrow->SetVisibility(1);

}


void ULockTargetComponent::SetRotationMode_FaceTarget()
{
	PlayerRef->bUseControllerRotationYaw = 0;
	PlayerRef->GetCharacterMovement()->bOrientRotationToMovement = 0;
	PlayerRef->GetCharacterMovement()->bUseControllerDesiredRotation = 0;
}


void ULockTargetComponent::CacheRotationSetting()
{
	bOwnerControllerRotationYaw = PlayerRef->bUseControllerRotationYaw;
	bOwnerOrientRotToMovement = PlayerRef->GetCharacterMovement()->bOrientRotationToMovement;
	bOwnerControllerDesiredRot = PlayerRef->GetCharacterMovement()->bUseControllerDesiredRotation;
}


void ULockTargetComponent::ResetRotationSetting()
{

	PlayerRef->bUseControllerRotationYaw = bOwnerControllerRotationYaw;
	PlayerRef->GetCharacterMovement()->bOrientRotationToMovement = bOwnerOrientRotToMovement;
	PlayerRef->GetCharacterMovement()->bUseControllerDesiredRotation = bOwnerControllerDesiredRot;
}

void ULockTargetComponent::DisableLockingTarget()
{
	bIsTargetingEnabled = 0;

	PlayerArrow->SetVisibility(0);

	if(SelectedActor)
		Cast<ITargetable>(SelectedActor)->ToggleLockIcon(0);

	SelectedActor = nullptr;

	ResetRotationSetting();

	GetOwner()->GetInstigatorController()->ResetIgnoreLookInput();
}


bool ULockTargetComponent::IsTraceBlocked(AActor *SelectedTarget, TArray<AActor*> IgnoredActors, const ECollisionChannel TraceChannel)
{

	TArray<FHitResult> Hits;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());
	QueryParams.AddIgnoredActors(IgnoredActors);

	//Rule-out targets where line trace from player to target is blocked in ECC_Camera channel
	if ((GetWorld()->LineTraceMultiByChannel(Hits, GetLineTraceStartLocation(), SelectedTarget->GetActorLocation(), TraceChannel, QueryParams)))
		return true;
	return false;
}


FVector ULockTargetComponent::GetLineTraceStartLocation()
{
	return (GetOwner()->GetActorLocation()
		+ GetOwner()->GetActorForwardVector() * 10.f
		+ GetOwner()->GetActorUpVector() * 100.f);
}


void ULockTargetComponent::Timer_CheckBlockingAndDistance()
{
	if (!bIsTargetingEnabled)
	{
		GetWorld()->GetTimerManager().ClearTimer(TargetBlockingHandler);
		return;
	}

	if (IsTraceBlocked(SelectedActor, TArray<AActor*>{SelectedActor}, ECC_WorldStatic))
	{
		DisableLockingTarget();
		return FindTarget();
	}

	float LocalDistance = FVector::Distance(GetOwner()->GetActorLocation(), SelectedActor->GetActorLocation());

	//Detect Distance
	if (LocalDistance <= 50.f || LocalDistance >= 2500.f)
	{
		DisableLockingTarget();
		return FindTarget();
	}

	if (!Cast<ITargetable>(SelectedActor)->IsTargetable())
	{
		DisableLockingTarget();
		return FindTarget();
	}
}


void ULockTargetComponent::Tick_UpdateRotation()
{
	if (!bIsTargetingEnabled) return;

	//Set Arrow Rotation
	if (PlayerArrow)
	{
		FRotator SlerpedRotation = GetOwner()->GetInstigatorController()->GetControlRotation();
		PlayerArrow->SetWorldRotation(FRotator{ 0.f, SlerpedRotation.Yaw, 0.f });
	}

	//Set Capsule Component rotation to face the target
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PlayerRef->GetActorLocation(),
		SelectedActor->GetActorLocation());

	if (!bFreeCamera)
	{
		FRotator LookedAtCameraRotation = FMath::RInterpConstantTo(PlayerRef->GetControlRotation(),
			LookAtRotation + FRotator{ -20.f, 0.f, 0.f }, GetWorld()->GetDeltaSeconds(), 300.f);

		PlayerRef->GetInstigator()->GetController()->SetControlRotation(LookedAtCameraRotation);
	}

	//TODO need a precise condition
	if (!PlayerRef->GetMesh()->GetAnimInstance()->IsAnyMontagePlaying() || bIsTargetingEnabled)
	{
		FRotator PlayerCapRotation = PlayerRef->GetCapsuleComponent()->GetComponentRotation();

		PlayerRef->GetCapsuleComponent()->SetWorldRotation(FMath::RInterpConstantTo(
			PlayerCapRotation,
			FRotator{ PlayerCapRotation.Pitch, LookAtRotation.Yaw, PlayerCapRotation.Roll },
			GetWorld()->GetDeltaSeconds(),
			800.f));
	}
}

