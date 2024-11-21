

#include "StartCharacter.h"

#include "AGoodStart/AGoodStartComponets/CombatComponent.h"
#include "AGoodStart/Weapon/Weapon.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

AStartCharacter::AStartCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	Combat->SetIsReplicated(true);

	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

void AStartCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(AStartCharacter, OverlappingWeapon, COND_OwnerOnly);
}


void AStartCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AStartCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AimOffset(DeltaTime);
	
}

void AStartCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AStartCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AStartCharacter::Look);
		EnhancedInputComponent->BindAction(ScreenLookAction, ETriggerEvent::Triggered, this, &AStartCharacter::ScreenLook);
		EnhancedInputComponent->BindAction(ScreenLookAction, ETriggerEvent::Started, this, &AStartCharacter::StartScreenLook);
		
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Started, this, &AStartCharacter::EquipButtonPressed);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this , &AStartCharacter::CrouchButtonPressed);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AStartCharacter::SetAimState);

		// EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AStartCharacter::AimButtonPressed);
		// EnhancedInputComponent->BindAction(AimAction,ETriggerEvent::Completed, this, &AStartCharacter::AimButtonReleased);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

	// PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//
	//  PlayerInputComponent->BindAxis("MoveForward", this, &AStartCharacter::MoveForward);
	//  PlayerInputComponent->BindAxis("MoveRight", this, &AStartCharacter::MoveRight);
	//  PlayerInputComponent->BindAxis("Turn", this, &AStartCharacter::Turn);
	//  PlayerInputComponent->BindAxis("LookUp", this, &AStartCharacter::LookUp);
	//
	// PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &AStartCharacter::EquipButtonPressed);
	// PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AStartCharacter::CrouchButtonPressed);
	//  PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AStartCharacter::AimButtonPressed);
	//  PlayerInputComponent->BindAction("Aim", IE_Released, this, &AStartCharacter::AimButtonReleased);
	//
	// PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &AStartCharacter::AttakcButtonPressed);
}

void AStartCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (Combat)
	{
		Combat->Character = this;
	}
}

void AStartCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr && MovementVector != FVector2D::ZeroVector)
	{
		const FRotator YawRotator(0.f, Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Direction_X(FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X));
		const FVector Direction_Y(FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction_X, MovementVector.Y);
		AddMovementInput(Direction_Y, MovementVector.X);
	}
}

void AStartCharacter::MoveForward(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotator(0.f, Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Direction(FRotationMatrix(YawRotator).GetUnitAxis(EAxis::X));
		AddMovementInput(Direction, Value);
	}
}

void AStartCharacter::MoveRight(float Value)
{
	if (Controller != nullptr && Value != 0.f)
	{
		const FRotator YawRotator(0.f, Controller->GetControlRotation().Yaw, 0.0f);
		const FVector Direction(FRotationMatrix(YawRotator).GetUnitAxis(EAxis::Y));
		AddMovementInput(Direction, Value);
	}
}

void AStartCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(-LookAxisVector.Y);
		//UE_LOG(LogTemp, Warning, TEXT("FVector2D: (%f, %f)"), LookAxisVector.X, LookAxisVector.Y);
	}
}

void AStartCharacter::ScreenLook(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D CurrentVector = Value.Get<FVector2D>();
	FVector2d LookAxisVector = CurrentVector - LastTouchVector;
	LastTouchVector = CurrentVector;
	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X * ScreenLookSensitivity);
		AddControllerPitchInput(LookAxisVector.Y * ScreenLookSensitivity);
	}
}

void AStartCharacter::StartScreenLook(const FInputActionValue& Value)
{
	// input is a Vector2D
	LastTouchVector = Value.Get<FVector2D>();
}

void AStartCharacter::LookUp(float Value)
{
	AddControllerPitchInput(Value);
}

void AStartCharacter::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void AStartCharacter::EquipButtonPressed()
{
	if (Combat)
	{
		if (HasAuthority())
		{
			Combat->EquipWeapon(OverlappingWeapon);	
		}
		else
		{
			ServerEquipButtonPressed();
		}
	}
}

void AStartCharacter::ServerEquipButtonPressed_Implementation()
{
	if (Combat)
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
}

void AStartCharacter::CrouchButtonPressed()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void AStartCharacter::SetAimState()
{
	if (Combat && Combat->EquippedWeapon)
	{
		Combat->SetAiming();
	}
}

// void AStartCharacter::AimButtonPressed()
// {
// 	if (Combat)
// 	{
// 		Combat->SetAiming(true);
// 	}
// }
//
// void AStartCharacter::AimButtonReleased()
// {
// 	if (Combat)
// 	{
// 		Combat->SetAiming(false);
// 	}
// }

void AStartCharacter::AimOffset(float DeltaTime)
{
	if (Combat && Combat->EquippedWeapon == nullptr) return;
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size();
	bool bIsInAir = GetCharacterMovement()->IsFalling();

	if (Speed == 0.f && !bIsInAir)
	{
		FRotator CurrentAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		FRotator DeltaAimRotation = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotation, StartingAimRotation);
		AO_Yaw = DeltaAimRotation.Yaw;
		bUseControllerRotationYaw = false;
	}
	if (Speed > 0.f || bIsInAir)
	{
		StartingAimRotation = FRotator(0.f, GetBaseAimRotation().Yaw, 0.f);
		AO_Yaw = 0.f;
		bUseControllerRotationYaw = true;
	}

	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.f && !IsLocallyControlled())
	{
		// map pitch from [270,360) to [-90,0)
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
}

void AStartCharacter::AttakButtonPressed()
{
	if (Combat && Combat->EquippedWeapon)
	{
		if (HasAuthority())
		{
			Combat->Attack();
		}
		else
		{
			ServerAttackButtonPressed();
		}
	}
}

void AStartCharacter::ServerAttackButtonPressed_Implementation()
{
	if (Combat && Combat->EquippedWeapon)
	{
		Combat->Attack();
	}
}

void AStartCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickWidget(false);
	}
	OverlappingWeapon = Weapon;
	if (IsLocallyControlled())
	{
		if (OverlappingWeapon)
		{
			OverlappingWeapon->ShowPickWidget(true);
		}
	}
}

void AStartCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickWidget(true);
	}
	if (LastWeapon)
	{
		LastWeapon->ShowPickWidget(false);
	}
}

bool AStartCharacter::IsWeaponEquipped()
{
	return (Combat && Combat->EquippedWeapon);
}

bool AStartCharacter::IsAiming()
{
	return (Combat && Combat->bAiming);
}

AWeapon* AStartCharacter::GetEquippedWeapon()
{
	if (Combat == nullptr) return nullptr;
	return Combat->EquippedWeapon;
}
