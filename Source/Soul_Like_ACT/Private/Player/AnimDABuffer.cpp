// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimDABuffer.h"

// Sets default values for this component's properties
UAnimDABuffer::UAnimDABuffer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = 0;

	// ...
}


// Called when the game starts
void UAnimDABuffer::BeginPlay()
{
	Super::BeginPlay();

	OwnerRef = Cast<ASoul_Like_ACTCharacter>(GetOwner());
}

void UAnimDABuffer::CalculateDmg(AWeaponActor * PlayerWeapon)
{

}

void UAnimDABuffer::CalculateForce(ATargetableActor * Target)
{
}

void UAnimDABuffer::GetCombo(EComboChoise ComboType)
{
}

bool UAnimDABuffer::PlayComboMontage()
{

}