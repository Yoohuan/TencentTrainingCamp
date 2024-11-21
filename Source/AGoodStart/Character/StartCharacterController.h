// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "StartCharacterController.generated.h"

/**
 * 
 */
UCLASS()
class AGOODSTART_API AStartCharacterController : public APlayerController
{
	GENERATED_BODY()

protected:

	/** Input Mapping Context to be used for player input */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* InputMappingContext;

	// Begin Actor interface

	virtual void BeginPlay() override;

	// End Actor interface
	
	
	
};
