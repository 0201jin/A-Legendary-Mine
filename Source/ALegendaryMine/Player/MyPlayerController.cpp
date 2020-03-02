// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"

AMyPlayerController::AMyPlayerController()
{
	bShowMouseCursor = true;
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = Cast<APlayerPawn>(GetPawn());
}

void AMyPlayerController::FB_Move(float _value)
{
	PlayerPawn->FB_Move(_value);
}

void AMyPlayerController::LR_Move(float _value)
{
	PlayerPawn->LR_Move(_value);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	InputComponent->BindAxis("FB_Move", this, &AMyPlayerController::FB_Move);
	InputComponent->BindAxis("LR_Move", this, &AMyPlayerController::LR_Move);
}