// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/AnimManager.h"
#include "TimerManager.h"
#include "Player/Soul_Like_ACTCharacter.h"
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
}

// Called every frame
void UAnimManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAnimManager::PlayMontage()
{
	switch (MyActionType)
	{
	case EActionType::Attack:
		PlayComboMontage();
		break;
	case EActionType::Dodge:
		PlayDodgeMontage();
		break;
	case  EActionType::Parry:
		PlayParryMontage_Pressed();
		break;
	default:
		break;
	}
}

void UAnimManager::PlayComboMontage()
{
	Cast<ACharacter>(GetOwner())->PlayAnimMontage(ComboMontages[ComboIndex], 1.f);
	IncreaseComboIndex();
}

void UAnimManager::IncreaseComboIndex()
{
	++ComboIndex;

	if (ComboIndex >= ComboMontages.Num())
	{
		ResetComboIndex();
	}
}

void UAnimManager::ResetCombo()
{
	SetAttackQueue(EAttackQueueStatus::Disabled);

	ResetComboIndex();
}


void UAnimManager::IncreasingChannelingPoints()
{
	++ChannelingPoints;
	//GetWorld()->GetTimerManager().SetTimer(ChannelingPointsTH, this, &UInputBuffComp::ResetChannelingPoints, .5f, 0, 3.f);
}

void UAnimManager::EnableActing()
{
	bIsActing = 1;
}

void UAnimManager::DisableActing()
{
	ResetCombo();
	bIsActing = 0;
}

void UAnimManager::TryUseDequeMotion(bool bTriggeredByAnimBP, FString & DebugMessage)
{

	if (bIsActing)
	{
		//Parry Montage Trigger
		if (bCanParry)
			PlayParryMontage_Pressed();

		if (AutoAttackQueue == EAttackQueueStatus::Disabled)
		{
			DebugMessage = "Is Attacking, Cant take input";
			return;
		}
		else if (AutoAttackQueue == EAttackQueueStatus::WaitForQueue)
		{
			if (bTriggeredByAnimBP)
			{
				SetAttackQueue(EAttackQueueStatus::ManualAttackOnly);
				DebugMessage = "Waiting for manual input";
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
				DebugMessage = "Attacking";
				return;
			}
			else
			{
				DebugMessage = "Queued, Cant take input";
			}
		}
		else if (AutoAttackQueue == EAttackQueueStatus::ManualAttackOnly
			&& !bTriggeredByAnimBP)
		{
			SetAttackQueue(EAttackQueueStatus::Disabled);
			PlayMontage();
			DebugMessage = "Manual trigger attacking";
			return;
		}
	}
	else if (
		AutoAttackQueue == EAttackQueueStatus::Disabled
		&& !bTriggeredByAnimBP)
	{
		PlayMontage();
		DebugMessage = "First attacking";
		return;
	}
	DebugMessage = "Do Nothing";
	return;
}

void UAnimManager::TryUseDequeMotion(const EActionType InpActionType, bool bTriggeredByAnimBP, FString &DebugMessage)
{
	SetActionType(InpActionType);
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

void UAnimManager::PlayParryMontage_Pressed()
{
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "PlayParryMontage_Pressed");

	if (bCanParry)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Try Parry");
		bCanParry = 0;
		GetWorld()->GetTimerManager().ClearTimer(Handle_WaitToParry);
		PlayerRef->PlayAnimMontage(ParryMontage);
	}
	else if (!bIsActing)
	{
		bCanParry = 1;

		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Try Block");

		ResetCombo();
		//PlayerRef->DisableInput(GetWorld()->GetFirstPlayerController());

		GetWorld()->GetTimerManager().SetTimer(Handle_WaitToParry, this, &UAnimManager::Timer_WaitToParry, 1.f, false, 0.3f);
		PlayerRef->PlayAnimMontage(BlockMontage);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Yellow, "Do Nothing");
	}
}

void UAnimManager::PlayParryMontage_Released()
{
	if (bIsActing && PlayerRef->GetCurrentMontage() == BlockMontage)
	{
		ResetCombo();

		PlayerRef->PlayAnimMontage(BlockMontage, 1.f, FName{ "Block_End" });
	}
}

void UAnimManager::Timer_WaitToParry()
{
	bCanParry = 0;
}

FString UAnimManager::GetQueueStatusMessage(EAttackQueueStatus const & Inp)
{
	switch (Inp)
	{
	case EAttackQueueStatus::Disabled:
		return "EAttackQueueStatus::Disabled";
	case EAttackQueueStatus::ManualAttackOnly:
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
	bIsStun = IsStun;

	if (IsStun)
	{
		PlayerRef->DisableInput(Cast<APlayerController>(PlayerRef->GetInstigatorController()));
	}
	else
	{
		PlayerRef->EnableInput(Cast<APlayerController>(PlayerRef->GetInstigatorController()));
	}

	OnStun_Delegate.Broadcast(bIsStun);
}

