// Fill out your copyright notice in the Description page of Project Settings.

#include "BpFunctionLib/MyBlueprintFunctionLibrary.h"
#include "GameFramework/Character.h"

void UMyBlueprintFunctionLibrary::ApplyTranslationScaleToCharacter(class ACharacter *Character, const float TransaltionScale)
{
	Character->SetAnimRootMotionTranslationScale(TransaltionScale);
}
