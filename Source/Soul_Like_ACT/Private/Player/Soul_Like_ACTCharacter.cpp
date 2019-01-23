// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "Player/Soul_Like_ACTCharacter.h"
#include "Player/AnimManager.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/InputComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Player/LockTargetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "GameFramework/Controller.h"
#include "Types/DamageType_MeleeHit.h"
#include "Types/DamageType_ParryRefelction.h"
#include "StatusComponent.h"
#include "GameFramework/SpringArmComponent.h"

const float ASoul_Like_ACTCharacter::BattleMovementScale{ .6f };
const float ASoul_Like_ACTCharacter::TravelMovementScale{ 1.f };

//////////////////////////////////////////////////////////////////////////
// ASoul_Like_ACTCharacter

ASoul_Like_ACTCharacter::ASoul_Like_ACTCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 1000.f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->MaxWalkSpeed = 800.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Arrow Component for Target Lock Component
	TargetLockArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("TargetLockArrow"));
	TargetLockArrow->SetupAttachment(RootComponent);
	TargetLockArrow->bAbsoluteRotation = 1;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	AnimManager = CreateDefaultSubobject<UAnimManager>(TEXT("AnimManager"));

	TargetLockingComponent = CreateDefaultSubobject<ULockTargetComponent>(TEXT("TargetLockingComponent"));

	Faction = EActorFaction::Player;

	AIPerceptionStimuliSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("AIPerceptionStimuliSource"));
}

void ASoul_Like_ACTCharacter::BeginPlay()
{
	Super::BeginPlay();

	TargetLockingComponent->InitComponent(TargetLockArrow);
}

void ASoul_Like_ACTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MakeMove();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ASoul_Like_ACTCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &ASoul_Like_ACTCharacter::UseLMB);
	PlayerInputComponent->BindAction("RMB", IE_Pressed, this, &ASoul_Like_ACTCharacter::UseRMB_Pressed);
	PlayerInputComponent->BindAction("RMB", IE_Released, this, &ASoul_Like_ACTCharacter::UseRMB_Released);
	PlayerInputComponent->BindAction("Space", IE_Pressed, this, &ASoul_Like_ACTCharacter::UseDodge);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASoul_Like_ACTCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASoul_Like_ACTCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASoul_Like_ACTCharacter::CalculateLeanValue);

	PlayerInputComponent->BindAxis("TurnRate", this, &ASoul_Like_ACTCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ASoul_Like_ACTCharacter::LookUpAtRate);

	PlayerInputComponent->BindAxis("Zoom", this, &ASoul_Like_ACTCharacter::ZoomCamera);
}


void ASoul_Like_ACTCharacter::ResetRotation()
{
	SetActorRotation(FRotator{ GetActorRotation().Pitch, GetInstigator()->GetViewRotation().Yaw,GetActorRotation().Roll });
}


void ASoul_Like_ACTCharacter::Exec_TryGetHit(float Damage, class UDamageType const* UDamageType, AController* EventInstigator, AActor* DamageCauser, EOnHitRefelction &Outp)
{
	if (UDamageType->GetClass() == UDamageType_MeleeHit::StaticClass())
	{
		if (Damage >= StatusComponent->MaxHealth)
		{
			UE_LOG(LogTemp, Warning, TEXT("Vulnerable"));
			Outp = EOnHitRefelction::Vulnerable;
			return;
		}
		else if (AnimManager->bIsParry)
		{
			UE_LOG(LogTemp, Warning, TEXT("Parry"));
			Outp = EOnHitRefelction::Parry;
			return;
		}
		else if (AnimManager->bIsBlocking)
		{
			UE_LOG(LogTemp, Warning, TEXT("Blocking"));
			Outp = EOnHitRefelction::Block;
			return;
		}
		else if (AnimManager->bIsDodging)
		{
			UE_LOG(LogTemp, Warning, TEXT("Dodging"));
			Outp = EOnHitRefelction::Immune;
			return;
		}
	}
	else if (UDamageType->GetClass() == UDamageType_ParryRefelction::StaticClass())
	{
		UE_LOG(LogTemp, Warning, TEXT("Vulnerable"));
		Outp = EOnHitRefelction::Vulnerable;
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("OnHit"));
	Outp = EOnHitRefelction::OnHit;
	return;
}

void ASoul_Like_ACTCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASoul_Like_ACTCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASoul_Like_ACTCharacter::UseLMB()
{
	if (Weapon)
	{
		FString DebugMessage;
		AnimManager->TryUseDequeMotion(EActionType::Attack, 0, DebugMessage);
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, DebugMessage);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "No Weapon");
	}
}

void ASoul_Like_ACTCharacter::UseRMB_Pressed()
{
	FString DebugMessage;
	AnimManager->TryUseDequeMotion(EActionType::Parry, 0, DebugMessage);
}

void ASoul_Like_ACTCharacter::UseRMB_Released()
{
	FString DebugMessage;
	AnimManager->PlayParryMontage_Released();
}

void ASoul_Like_ACTCharacter::ZoomCamera(float Rate)
{
	float &ArmLength = CameraBoom->TargetArmLength;
	ArmLength += Rate * -100.f;
}

void ASoul_Like_ACTCharacter::UseDodge()
{
	FString DebugMessage;
	AnimManager->TryUseDequeMotion(EActionType::Dodge, 0, DebugMessage);
}

void ASoul_Like_ACTCharacter::CalculateLeanValue(float TurnValue)
{
	if (TargetLockingComponent->GetIsTargetingEnabled())
	{
		LeanAmount_Char = 0.f;
		LeanSpeed_Char = 10.f;		
	}
	else
	{
		LeanAmount_Char = TurnValue;
		LeanSpeed_Char = 1.f;
	}

	LeanAmount_Anim = FMath::FInterpTo(LeanAmount_Anim, LeanAmount_Char, GetWorld()->GetDeltaSeconds(), LeanSpeed_Char);
	
	GetLaneAmountDelegate.Broadcast(LeanAmount_Anim);
}

void ASoul_Like_ACTCharacter::MoveForward(float Value)
{
	//Axis Value for AnimManager
	ForwardAxisValue = Value;
}

void ASoul_Like_ACTCharacter::MoveRight(float Value)
{
	//Get axis value for AnimManager
	RightAxisValue = Value;
}

void ASoul_Like_ACTCharacter::MakeMove()
{
	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();

		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction =
			(FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X) * ForwardAxisValue
				+ FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y) * RightAxisValue).GetSafeNormal();

		if (TargetLockingComponent->GetIsTargetingEnabled())
			AddMovementInput(Direction, BattleMovementScale);
		else
			AddMovementInput(Direction, TravelMovementScale);
	}
}
