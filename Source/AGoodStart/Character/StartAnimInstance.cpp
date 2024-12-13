
#include "StartAnimInstance.h"
#include "StartCharacter.h"
#include "AGoodStart/Weapon/Weapon.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UStartAnimInstance::NativeInitializeAnimation() 
{
    Super::NativeInitializeAnimation();

    StartCharacter = Cast<AStartCharacter>(TryGetPawnOwner());
}

void UStartAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (StartCharacter == nullptr)
    {
        StartCharacter = Cast<AStartCharacter>(TryGetPawnOwner());
    }
    if (StartCharacter == nullptr) return;

    FVector Velocity = StartCharacter->GetVelocity();
    Velocity.Z = 0.f;
    Speed = Velocity.Size();

    bIsInAir = StartCharacter->GetCharacterMovement()->IsFalling();
    bIsAccelerating = StartCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f ? true : false;
    bWeaponEquipped = StartCharacter->IsWeaponEquipped();
    EquippedWeapon = StartCharacter->GetEquippedWeapon();
    bIsCrouched = StartCharacter->bIsCrouched;
    bAiming = StartCharacter->IsAiming();
    TurningInPlace = StartCharacter->GetTurningInPlace();
    bRotateRootBone = StartCharacter->ShouldRotateRootBone();

    // Offset Yaw for Strafing
    FRotator AimRotation = StartCharacter->GetBaseAimRotation();
    FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(StartCharacter->GetVelocity());
    FRotator DeltaRot = UKismetMathLibrary::NormalizedDeltaRotator(MovementRotation,AimRotation);
    DeltaRotation = FMath::RInterpTo(DeltaRotation, DeltaRot, DeltaTime, 6.f);
    YawOffset = DeltaRotation.Yaw;

    CharacterRotatorLastFrame = CharacterRotator;
    CharacterRotator = StartCharacter->GetActorRotation();
    const FRotator Delta = UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotator,CharacterRotatorLastFrame);
    const float Target = Delta.Yaw / DeltaTime;
    const float Interp = FMath::FInterpTo(Lean, Target, DeltaTime, 6.f);
    Lean = FMath::Clamp(Interp, -90.f, 90.f);

    AO_Yaw = StartCharacter->GetAO_Yaw();
    AO_Pitch = StartCharacter->GetAO_Pitch();

    if (bWeaponEquipped && EquippedWeapon && EquippedWeapon->GetWeaponMesh() && StartCharacter->GetMesh())
    {
        LeftHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHandSocket"), RTS_World);
        FVector OutPosition;
        FRotator OutRotation;
        StartCharacter->GetMesh()->TransformToBoneSpace(FName("hand_r"), LeftHandTransform.GetLocation(), FRotator::ZeroRotator, OutPosition, OutRotation);
        LeftHandTransform.SetLocation(OutPosition);
        LeftHandTransform.SetRotation(FQuat(OutRotation));

        if (StartCharacter->IsLocallyControlled())
        {
            bLocallyControlled = true;
            FTransform RightHandTransform = EquippedWeapon->GetWeaponMesh()->GetSocketTransform(FName("hand_r"), RTS_World);
            FRotator LookARotation = UKismetMathLibrary::FindLookAtRotation(RightHandTransform.GetLocation(), RightHandTransform.GetLocation() + (RightHandTransform.GetLocation() - StartCharacter->GetHitTarget()));
            RightHandRotation = FMath::RInterpTo(RightHandRotation, LookARotation, DeltaTime, 30.f);
        }
        
    }
}


