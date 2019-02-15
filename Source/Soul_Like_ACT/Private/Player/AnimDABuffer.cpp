// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimDABuffer.h"
#include "Player/AnimManager.h"
#include "Types/DA_ComboMontage.h"
#include "Animation/AnimMontage.h"
#include "Types/DA_PlayerAnimSet.h"
#include "Player/Soul_Like_ACTCharacter.h"


void UAnimDABuffer::ApplyComboDA(UAnimManager *AnimManagerRef, bool bResetCombo /*= 0*/)
{
	if (!bResetCombo && AnimManagerRef->CurrentComboStage->Combo_DA)
	{
		AnimManagerRef->CurrentComboStage = AnimManagerRef->CurrentComboStage->Combo_DA;
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "UAnimDABuffer combo applied");
	}
	else
	{
		AnimManagerRef->CurrentComboStage = AnimManagerRef->DefaultAnimSet->ComboSet1;
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "UAnimDABuffer combo inited");
	}
}

class UAnimMontage * UAnimDABuffer::GetAnimMontageFromAttackDA(UDA_AttackMontage *AttackDA, EComboChoise ELightOrHeavy)
{
	switch (ELightOrHeavy)
	{
	case EComboChoise::LightAttack:
		return AttackDA->Normal_Montage;
	case EComboChoise::HeavyAttack:
		if (AttackDA->Heavy_Montage)
			return AttackDA->Heavy_Montage;
		else 
			return AttackDA->Normal_Montage;
	default:
		return nullptr;
	}
}

class UAnimMontage * UAnimDABuffer::GetPreMontageFromAttackDA(UDA_AttackMontage *AttackDA)
{
	return AttackDA->Pre_Montage;
}

class UAnimMontage * UAnimDABuffer::GetAnimMontageFromUtilityDA(UDA_UtilityMontage *UtilityDA)
{
	return UtilityDA->Normal_Montage;
}
