// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AGoodStart/StartTypes/TurningInPlace.h"
#include "AGoodStart/Interfaces/InteractWithCrosshairsInterface.h"
#include "StartCharacter.generated.h"

struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
UCLASS()
class AGOODSTART_API AStartCharacter : public ACharacter ,public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()

public:
	AStartCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;
	void PlayFireMontage(bool bAiming);

	UFUNCTION(NetMulticast, Unreliable)
	void MulticastHit();

	virtual void OnRep_ReplicateMovement() override;
protected:
	virtual void BeginPlay() override;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ScreenLook(const FInputActionValue& Value);
	void StartScreenLook(const FInputActionValue& Value);
	void MoveForward(float Value);
	void MoveRight(float Value);
	void LookUp(float Value);
	void Turn(float Value);
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void SetAimState();
	void CalculateAO_Pitch();
	// void AimButtonPressed();
	// void AimButtonReleased();
	void AimOffset(float DeltaTime);
	void SimProxiesTurn();
	virtual void Jump() override;
	void FireButtonPressed();
	void FireButtonReleased();
	void PlayHitReactMontage();
	
private:
	UPROPERTY(EditAnywhere, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, Category = Input, meta=(AllowPrivateAccess=true))
    class UInputAction* ScreenLookAction;

	UPROPERTY(EditAnywhere, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* AimAction;

	UPROPERTY(EditAnywhere, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, Category = Input, meta=(AllowPrivateAccess=true))
	class UInputAction* FireAction;
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* CameraBoom;
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* OverheadWidget;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon)
	class AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;
	
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed();

	UPROPERTY(EditAnywhere, Category = Sensitivity)
	float ScreenLookSensitivity;

	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;
	FVector2D LastTouchVector;

	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	UPROPERTY(EditAnywhere, Category = Combat)
	class UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactMontage;

	void HideCameraIfCharacterClose();

	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;

	bool bRotateRootBone;
	float TurnThreshold = 20.f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	float CalculateSpeed();

public:	
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped();
	bool IsAiming();
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	AWeapon* GetEquippedWeapon();
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FVector GetHitTarget() const;
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
};
