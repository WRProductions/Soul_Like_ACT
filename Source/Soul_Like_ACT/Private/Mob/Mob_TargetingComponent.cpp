// Fill out your copyright notice in the Description page of Project Settings.

#include "Mob_TargetingComponent.h"
#include "Mob/MobBasic.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UMob_TargetingComponent::UMob_TargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = 1;
	//PrimaryComponentTick.bStartWithTickEnabled = 1;

	// ...
}


void UMob_TargetingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	//Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, "UMob_TargetingComponent::TickComponent successful");

	if (bIsFacingTarget && !OwnerRef->GetIsStun())
	{
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
}

void UMob_TargetingComponent::FacingTarget_Init()
{
	if (TargetPawn)
	{
		bIsFacingTarget = 1;
	}
	else
		UE_LOG(LogTemp, Warning, TEXT("UMob_TargetingComponent::FacingTarget_Init failed"));
}

void UMob_TargetingComponent::FacingTarget_End()
{
	bIsFacingTarget = 0;
}

void UMob_TargetingComponent::ToggleTargetLocking()
{
	if (bIsFacingTarget)
		FacingTarget_End();
	else if (TargetPawn)
		FacingTarget_Init();
}
