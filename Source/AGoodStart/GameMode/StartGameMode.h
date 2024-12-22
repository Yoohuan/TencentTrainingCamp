// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "StartGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AGOODSTART_API AStartGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void PlayerEliminated(class AStartCharacter* ElimmedCharacter, class AStartPlayerController* VictimController, AStartPlayerController* AttackerController);
	virtual void RequestRespawn(class ACharacter* ElimmedCharacter, AController* ElimmedController);
	
};
