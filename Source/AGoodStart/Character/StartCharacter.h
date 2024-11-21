// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StartCharacter.generated.h"

struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);
UCLASS()
class AGOODSTART_API AStartCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AStartCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

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
	// void AimButtonPressed();
	// void AimButtonReleased();
	void AimOffset(float DeltaTime);
	void AttakButtonPressed();
	
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

	UFUNCTION(Server, Reliable)
	void ServerAttackButtonPressed();

	UPROPERTY(EditAnywhere, Category = Sensitivity)
	float ScreenLookSensitivity;

	float AO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;
	FVector2D LastTouchVector;

public:	
	void SetOverlappingWeapon(AWeapon* Weapon);
	bool IsWeaponEquipped();
	bool IsAiming();
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	AWeapon* GetEquippedWeapon();
};
