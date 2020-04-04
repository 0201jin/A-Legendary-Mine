// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Player/PlayerPawn.h"

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ALEGENDARYMINE_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

		AMyPlayerController();

public:
	virtual void SetupInputComponent();
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

protected:
	void FB_Move(float _value);
	void LR_Move(float _value);

	void Jump();
	void Attack(float _value);

protected:
	APlayerPawn * PlayerPawn;
};
