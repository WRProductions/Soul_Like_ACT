// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AI_Enums.generated.h"

UENUM(BlueprintType)
enum class EAgreesiveActions : uint8
{
	WalkAround,
	RunAttack,
	DashAttack,
	DashBack,
};

UENUM(BlueprintType)
enum class EDefensiveActions : uint8
{
	WalkAround,
	RunToPlayer,
	RunToPlayer2,
	RunToPlayer3,
};