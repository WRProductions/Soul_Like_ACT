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
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UAnimManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef = Cast<ASoul_Like_ACTCharacter>(GetOwner());

	//CurrentCombo is by default the default anim set
	UAnimDABuffer::ApplyComboDA(this, 1);
}

// Called every frame
void UAnimManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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
	UAnimMontage *LocalMontage = UAnimDABuffer::GetAnimMontageFromAttackDA(
		CurrentComboStage,
		EComboChoise::LightAttack);
	Cast<ACharacter>(GetOwner())->PlayAnimMontage(nullptr, 1.f);
	//Cast<ACharacter>(GetOwner())->PlayAnimMontage(CurrentComboStage->Normal_Montage, 1.f);
	GetNextAttackDA();
}


void UAnimManager::PlayHeavyAttack()
{
	UAnimMontage *LocalMontage = UAnimDABuffer::GetAnimMontageFromAttackDA(
		CurrentComboStage,
		EComboChoise::HeavyAttack);
	Cast<ACharacter>(GetOwner())->PlayAnimMontage(CurrentComboStage->Heavy_Montage, 1.f);
	GetNextAttackDA();
}


void UAnimManager::GetNextAttackDA()
{
	UAnimDABuffer::ApplyComboDA(this, 0);
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

void UAnimManager::EnableActing()
{
	bStopMovement = 1;
}

void UAnimManager::EndActing()
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
				SetAttackQueue(EAttackQueueStatus::ManualInputOnly);
				return;
			}
			//Triggered Manually
			else
			{
				SetAttackQueue(EAttackQueueStatus::Queued);
				DebugMessage = "Queued";
				return;
			}
		}
		else if (AutoAttackQueue == EAttackQueueStatus::Queued)
		{
			if (bTriggeredByAnimBP)
			{
				SetAttackQueue(EAttackQueueStatus::Disabled);
				
				PlayMontage();
				DebugMessage = "Anim triggered attacking";
				return;
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
			SetAttackQueue(EAttackQueueStatus::Disabled);
			PlayMontage();
			DebugMessage = "Manual triggered attacking";
			return;
		}
	}
	if (AutoAttackQueue == EAttackQueueStatus::Disabled
		&& !bTriggeredByAnimBP)
	{
		PlayMontage();
		DebugMessage = "First attacking";
		return;
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

		PlayerRef->PlayAnimMontage(Dash_Forward);
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

		PlayerRef->PlayAnimMontage(Dash_Forward);
	}
}

void UAnimManager::PlayBlockOrParry()
{
	if (bCanCastParry)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Try Parry");
		GetWorld()->GetTimerManager().SetTimer(Handle_WaitToParry, this, &UAnimManager::Timer_ResetCanTriggerParry,1.f, false, -1.f);
		PlayerRef->PlayAnimMontage(ParryMontage);
	}
	else if (GetCanMove())
	{

		ResetCombo();

		bCanCastParry = 1;
		GetWorld()->GetTimerManager().SetTimer(Handle_WaitToParry, this, &UAnimManager::Timer_ResetCanTriggerParry, 1.f, false, 0.3f);
		
		PlayerRef->PlayAnimMontage(BlockMontage);
	}
}


void UAnimManager::PlayParryMontage_Released()
{
	if (!GetCanMove() && PlayerRef->GetCurrentMontage() == BlockMontage)
	{
		ResetCombo();

		PlayerRef->PlayAnimMontage(BlockMontage, 1.f, FName{ "Block_End" });
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

