// Fill out your copyright notice in the Description page of Project Settings.


#include "StartPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "AGoodStart/HUD/StartHUD.h"
#include "AGoodStart/HUD/CharacterOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

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



