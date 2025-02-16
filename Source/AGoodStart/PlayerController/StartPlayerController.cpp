// Fill out your copyright notice in the Description page of Project Settings.


#include "StartPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "AGoodStart/HUD/StartHUD.h"
#include "AGoodStart/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "AGoodStart/Character/StartCharacter.h"

void AStartPlayerController::BeginPlay()
{
	Super::BeginPlay();

	StartHUD = Cast<AStartHUD>(GetHUD());

	// get the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add the mapping context so we get controls
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void AStartPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AStartCharacter* StartCharacter = Cast<AStartCharacter>(InPawn);
	if (StartCharacter)
	{
		SetHUDHealth(StartCharacter->GetHealth(), StartCharacter->GetMaxHealth());
	}
}

void AStartPlayerController::SetHUDHealth(float Health, float MaxHealth)
{
	StartHUD = StartHUD == nullptr ? Cast<AStartHUD>(GetHUD()) : StartHUD;

	bool bHUDValid = StartHUD &&
		StartHUD->CharacterOverlay &&
		StartHUD->CharacterOverlay->HealthBar &&
		StartHUD->CharacterOverlay->HealthText;
	if (bHUDValid)
	{
		const float HealthPercent = Health / MaxHealth;
		StartHUD->CharacterOverlay->HealthBar->SetPercent(HealthPercent);
		FString HealthText = FString::Printf(TEXT("%d/%d"), FMath::CeilToInt(Health), FMath::CeilToInt(MaxHealth));
		StartHUD->CharacterOverlay->HealthText->SetText(FText::FromString(HealthText));
	}
}

void AStartPlayerController::SetHUDScore(float Score)
{
	StartHUD = StartHUD == nullptr ? Cast<AStartHUD>(GetHUD()) : StartHUD;

	bool bHUDValid = StartHUD &&
		StartHUD->CharacterOverlay &&
		StartHUD->CharacterOverlay->ScoreAmount;
	if (bHUDValid)
	{
		FString ScoreText = FString::Printf(TEXT("%d"), FMath::FloorToInt(Score));
		StartHUD->CharacterOverlay->ScoreAmount->SetText(FText::FromString(ScoreText));
	}
}

void AStartPlayerController::SetHUDDefeats(int32 Defeats)
{
	StartHUD = StartHUD == nullptr ? Cast<AStartHUD>(GetHUD()) : StartHUD;

	bool bHUDValid = StartHUD &&
		StartHUD->CharacterOverlay &&
		StartHUD->CharacterOverlay->DefeatsAmount;
	if (bHUDValid)
	{
		FString DefeatsText = FString::Printf(TEXT("%d"), Defeats);
		StartHUD->CharacterOverlay->DefeatsAmount->SetText(FText::FromString(DefeatsText));
	}
}

void AStartPlayerController::SetHUDWeaponAmmo(int32 Ammo)
{
	StartHUD = StartHUD == nullptr ? Cast<AStartHUD>(GetHUD()) : StartHUD;

	bool bHUDValid = StartHUD &&
		StartHUD->CharacterOverlay &&
		StartHUD->CharacterOverlay->WeaponAmmoAmount;
	if (bHUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		StartHUD->CharacterOverlay->WeaponAmmoAmount->SetText(FText::FromString(AmmoText));
	}
}

void AStartPlayerController::SetHUDCarriedAmmo(int32 Ammo)
{
	StartHUD = StartHUD == nullptr ? Cast<AStartHUD>(GetHUD()) : StartHUD;

	bool bHUDValid = StartHUD &&
		StartHUD->CharacterOverlay &&
		StartHUD->CharacterOverlay->CarriedAmmoAmount;
	if (bHUDValid)
	{
		FString AmmoText = FString::Printf(TEXT("%d"), Ammo);
		StartHUD->CharacterOverlay->CarriedAmmoAmount->SetText(FText::FromString(AmmoText));
	}
}



