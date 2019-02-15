// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/AnimManager.h"
#include "Player/Soul_Like_ACTCharacter.h"
#include "Player/AnimDABuffer.h"
#include "Types/DA_PlayerAnimSet.h"
#include "Types/DA_ComboMontage.h"
#include "TimerManager.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UAnimManager::UAnimManager()
{
	PrimaryComponentTick.bCanEverTick = 0;
}


void UAnimManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<ASoul_Like_ACTCharacter>(GetOwner());

	//CurrentCombo is by default the default anim set
	UAnimDABuffer::ApplyComboDA(this, 1);
}


void UAnimManager::PlayMontage()
{
	switch (InputState)
	{
	case EInputState::Attack_Light:
		PlayLightAttack();
		break;
	case EInputState::Attack_Heavy:
		PlayHeavyAttack();
		break;
	case EInputState::Attack_Pre:
		PlayPreAttack();
		break;
	case EInputState::Dodge:
		PlayDodgeMontage();
		break;
	case  EInputState::Block:
		PlayBlockOrParry();
		break;
	default:
		break;
	}
}


void UAnimManager::PlayPreAttack()
{
	Cast<ACharacter>(GetOwner())->PlayAnimMontage(CurrentComboStage->Pre_Montage);
}


void UAnimManager::PlayLightAttack()
{
	Cast<ACharacter>(GetOwner())->PlayAnimMontage(CurrentComboStage->Normal_Montage, 1.f);
	GetNextAttackDA();
}


void UAnimManager::PlayHeavyAttack()
{
	Cast<ACharacter>(GetOwner())->PlayAnimMontage(CurrentComboStage->Heavy_Montage, 1.f);
	GetNextAttackDA();
}


void UAnimManager::GetNextAttackDA()
{
	UAnimDABuffer::ApplyComboDA(this, 0);
}

void UAnimManager::EnableLightAttackQueue()
{
	if (bLightAttackQueued)
		return TriggerLightAttackFromQueue();
	else
	{
		bCanLightAttack = 1;
		return;
	}
}

void UAnimManager::TriggerLightAttackFromQueue()
{
	PlayerRef->bIsLeftMouseButtonPressed = 0;

	bLightAttackQueued = 0;
	bCanLightAttack = 0;
	FString Local;

	InputState = EInputState::Attack_Light;
	PlayMontage();
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Green, Local);
}

void UAnimManager::TriggerHeavyAttack()
{
	PlayerRef->bIsLeftMouseButtonPressed = 0;

	bLightAttackQueued = 0;
	bCanLightAttack = 0;
	FString Local;

	InputState = EInputState::Attack_Heavy;
	PlayMontage();
}

void UAnimManager::TryTriggerAttack()
{
	if (!bCanLightAttack)
		bLightAttackQueued = 1;
	else
		TriggerLightAttackFromQueue();
}

void UAnimManager::ResetCombo()
{
	SetAttackQueue(EAttackQueueStatus::Disabled);

	ResetComboIndex();
}


void UAnimManager::ResetComboIndex()
{
	UAnimDABuffer::ApplyComboDA(this, 1);
}


void UAnimManager::IncreasingChannelingPoints()
{
	++ChannelingPoints;
	//GetWorld()->GetTimerManager().SetTimer(ChannelingPointsTH, this, &UInputBuffComp::ResetChannelingPoints, .5f, 0, 3.f);
}


void UAnimManager::DisableMovementInput()
{
	bStopMovement = 1;
}


void UAnimManager::EnableMovementInput()
{
	ResetCombo();
	bStopMovement = 0;
}


void UAnimManager::TryUseDequeMotion(bool bTriggeredByAnimBP, FString & DebugMessage)
{
	if (bStopMovement)
	{
		if (AutoAttackQueue == EAttackQueueStatus::Disabled)
		{
			DebugMessage = "Is Attacking, Cant take input";
			return;
		}
		else if (AutoAttackQueue == EAttackQueueStatus::WaitForQueue)
		{
			if (bTriggeredByAnimBP)
			{
				DebugMessage = "Mannual Input enabled";

				return SetAttackQueue(EAttackQueueStatus::ManualInputOnly);
			}
			//Triggered Manually
			else
			{
				DebugMessage = "Queued";

				return SetAttackQueue(EAttackQueueStatus::Queued);
			}
		}
		else if (AutoAttackQueue == EAttackQueueStatus::Queued)
		{
			if (bTriggeredByAnimBP)
			{
				DebugMessage = "Anim triggered attacking";

				SetAttackQueue(EAttackQueueStatus::Disabled);
				
				return PlayMontage();
			}
			else
			{
				DebugMessage = "Queued, Cant take input";
				
				return;
			}
		}
		else if (AutoAttackQueue == EAttackQueueStatus::ManualInputOnly
			&& !bTriggeredByAnimBP)
		{
			DebugMessage = "Manual triggered attacking";
			
			SetAttackQueue(EAttackQueueStatus::Disabled);
			
			return PlayMontage();
		}
	}

	if (AutoAttackQueue == EAttackQueueStatus::Disabled
		&& !bTriggeredByAnimBP)
	{
		DebugMessage = "First attacking";

		return PlayMontage();
	}
	DebugMessage = "Do Nothing";
	return;
}


void UAnimManager::TryUseDequeMotion(const EInputState InpActionType, bool bTriggeredByAnimBP, FString &DebugMessage)
{
	InputState = InpActionType;
	TryUseDequeMotion(bTriggeredByAnimBP, DebugMessage);
}


void UAnimManager::PlayDodgeMontage()
{
	ResetCombo();

	FVector2D AxisVec2D{ PlayerRef->ForwardAxisValue, PlayerRef->RightAxisValue };
	FRotator CamForwardRotation = FRotator{ PlayerRef->GetActorRotation().Pitch, PlayerRef->GetInstigator()->GetControlRotation().Yaw, PlayerRef->GetActorRotation().Roll };

	if (AxisVec2D.Size() <= 0.01f)
	{
		PlayerRef->SetActorRotation(CamForwardRotation);

		PlayerRef->PlayAnimMontage(UAnimDABuffer::GetAnimMontageFromUtilityDA(DefaultAnimSet->Dodge_F));
	}
	else
	{
		FRotator ControlledRotation = PlayerRef->GetInstigator()->GetControlRotation();

		FVector ForwardVec = FRotationMatrix(ControlledRotation).GetScaledAxis(EAxis::X) * PlayerRef->ForwardAxisValue * 100.f;
		FVector RightVec = FRotationMatrix(ControlledRotation).GetScaledAxis(EAxis::Y) * PlayerRef->RightAxisValue * 100.f;
		ForwardVec.Z = 0;
		RightVec.Z = 0;

		FVector LookAtRotation = PlayerRef->GetActorLocation() + ForwardVec + RightVec;

		FRotator DodgeDir = UKismetMathLibrary::FindLookAtRotation(PlayerRef->GetActorLocation(), LookAtRotation);

		PlayerRef->SetActorRotation(DodgeDir);

		PlayerRef->PlayAnimMontage(UAnimDABuffer::GetAnimMontageFromUtilityDA(DefaultAnimSet->Dodge_F));
	}
}


void UAnimManager::PlayBlockOrParry()
{
	if (bCanCastParry)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Try Parry");
		GetWorld()->GetTimerManager().SetTimer(Handle_WaitToParry, this, &UAnimManager::Timer_ResetCanTriggerParry,1.f, false, -1.f);
		PlayerRef->PlayAnimMontage(UAnimDABuffer::GetAnimMontageFromUtilityDA(DefaultAnimSet->Parry));
	}
	else if (GetCanMove())
	{
		ResetCombo();

		bCanCastParry = 1;
		GetWorld()->GetTimerManager().SetTimer(Handle_WaitToParry, this, &UAnimManager::Timer_ResetCanTriggerParry, 1.f, false, 0.3f);
		
		PlayerRef->PlayAnimMontage(UAnimDABuffer::GetAnimMontageFromUtilityDA(DefaultAnimSet->Block));
	}
}


void UAnimManager::PlayParry_Try()
{

}


void UAnimManager::PlayParryMontage_Released()
{
	if (!GetCanMove() && PlayerRef->GetCurrentMontage() == UAnimDABuffer::GetAnimMontageFromUtilityDA(DefaultAnimSet->Block))
	{
		ResetCombo();

		PlayerRef->PlayAnimMontage(UAnimDABuffer::GetAnimMontageFromUtilityDA(DefaultAnimSet->Block), 1.f, FName{ "Block_End" });
	}
}


void UAnimManager::Timer_ResetCanTriggerParry()
{
	bCanCastParry = 0;
}


FString UAnimManager::GetQueueStatusMessage(EAttackQueueStatus const & Inp)
{
	switch (Inp)
	{
	case EAttackQueueStatus::Disabled:
		return "EAttackQueueStatus::Disabled";
	case EAttackQueueStatus::ManualInputOnly:
		return "EAttackQueueStatus::ManualAttackOnly";
	case EAttackQueueStatus::Queued:
		return "EAttackQueueStatus::Queued";
	case EAttackQueueStatus::WaitForQueue:
		return "EAttackQueueStatus::WaitForQueue";
	default:
		return "Invalid Input";
	}
}


void UAnimManager::SetIsStun(bool IsStun)
{
	if (IsStun)
	{
		InputState = EInputState::OnStun;

		PlayerRef->DisableInput(Cast<APlayerController>(PlayerRef->GetInstigatorController()));
	}
	else
	{
		InputState = EInputState::Loco;

		PlayerRef->EnableInput(Cast<APlayerController>(PlayerRef->GetInstigatorController()));
	}

}