// Fill out your copyright notice in the Description page of Project Settings.

#include "Mob_TargetingComponent.h"

// Sets default values for this component's properties
UMob_TargetingComponent::UMob_TargetingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = 0;

	// ...
}


// Called when the game starts
void UMob_TargetingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UMob_TargetingComponent::ToggleTargetLocking()
{

}
