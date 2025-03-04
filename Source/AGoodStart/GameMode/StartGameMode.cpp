// Fill out your copyright notice in the Description page of Project Settings.


#include "StartGameMode.h"
#include "AGoodStart/Character/StartCharacter.h"
#include "AGoodStart/PlayerController/StartPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "AGoodStart/PlayerState/StartPlayerState.h"

void AStartGameMode::PlayerEliminated(class AStartCharacter* ElimmedCharacter,
	class AStartPlayerController* VictimController, AStartPlayerController* AttackerController)
{
	AStartPlayerState* AttackerPlayerState = AttackerController ? Cast<AStartPlayerState>(AttackerController->PlayerState) : nullptr;
	AStartPlayerState* VictimPlayerState = VictimController ? Cast<AStartPlayerState>(VictimController->PlayerState) : nullptr;

	if (AttackerPlayerState && AttackerPlayerState != VictimPlayerState)
	{
		AttackerPlayerState->AddToScore(1.f);
	}
	if (VictimPlayerState)
	{
		VictimPlayerState->AddToDefeats(1);
	}
	
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Elim();
	}
}

void AStartGameMode::RequestRespawn(class ACharacter* ElimmedCharacter, AController* ElimmedController)
{
	if (ElimmedCharacter)
	{
		ElimmedCharacter->Reset();
		ElimmedCharacter->Destroy();
	}
	if (ElimmedController)
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(this, APlayerStart::StaticClass(), PlayerStarts);
		int32 Selection = FMath::RandRange(0, PlayerStarts.Num() - 1);
		RestartPlayerAtPlayerStart(ElimmedController, PlayerStarts[Selection]);
	}
}
