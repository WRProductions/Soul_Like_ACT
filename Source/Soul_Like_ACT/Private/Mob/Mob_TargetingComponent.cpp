// Fill out your copyright notice in the Description page of Project Settings.

#include "Mob_TargetingComponent.h"
#include "Mob/MobBasic.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UMob_TargetingComponent::UMob_TargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	//PrimaryComponentTick.bStartWithTickEnabled = 1;
}


void UMob_TargetingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "UMob_TargetingComponent::TickComponent successful");

	if(FacingPriority == EFacingPriority::Target){
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), TargetPawn->GetActorLocation());
		//TODO: Set rotate speed
		FRotator Interped_LookAtRotation = FMath::RInterpConstantTo(GetOwner()->GetActorRotation(), LookAtRotation, DeltaTime, 300.f);

		GetOwner()->SetActorRotation(Interped_LookAtRotation);
	}
		
}

// Called when the game starts
void UMob_TargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	check(OwnerRef);
}

void UMob_TargetingComponent::SetMoveSpeedCoe(bool bEnabled)
{
	if (bEnabled)
		MoveSpeedCoe = .7f;
	else
		MoveSpeedCoe = 1.0f;

	OwnerRef->HandleMoveSpeedChanged(FOnAttributeChangeData());
}

void UMob_TargetingComponent::SetFacingPriority(EFacingPriority InFacingPriority)
{
	FacingPriority = InFacingPriority;

	UCharacterMovementComponent* LocalCharMovement = OwnerRef->GetCharacterMovement();
	check(LocalCharMovement);

	switch (FacingPriority)
	{
	case EFacingPriority::Target:
		LocalCharMovement->bOrientRotationToMovement = false;
		break;
	case EFacingPriority::MovingDirection:
		LocalCharMovement->bOrientRotationToMovement = true;
		break;
	default:
		LOG_FUNC_ERROR("Invalid EFacingPriority Input");
		break;
	}
}

void UMob_TargetingComponent::FacingTarget_Init(AActor* TargetActor, EFacingPriority InFacingPriority /*= EFacingPriority::Target*/)
{
	TargetPawn = TargetActor;

	if (TargetPawn)
	{
		SetFacingPriority(InFacingPriority);
		SetMoveSpeedCoe(true);
	}
	else
	{
		SetMoveSpeedCoe(false);
		SetFacingPriority(EFacingPriority::MovingDirection);
		UE_LOG(LogTemp, Warning, TEXT("UMob_TargetingComponent::FacingTarget_Init failed"));
	}
}

void UMob_TargetingComponent::FacingTarget_End()
{
	SetTarget(nullptr);
	SetMoveSpeedCoe(false);
	SetFacingPriority(EFacingPriority::MovingDirection);
}
