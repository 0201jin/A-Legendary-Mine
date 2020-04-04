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

void AMyPlayerController::Jump()
{
	PlayerPawn->Jump();
}

void AMyPlayerController::Attack(float _value)
{
	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, HitResult);
	FRotator DiRo = (HitResult.Location - PlayerPawn->GetActorLocation()).Rotation();
	DiRo.Pitch = 0;
	DiRo.Roll = 0;

	if (_value >= 1)
		PlayerPawn->Attack(DiRo);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAxis("FB_Move", this, &AMyPlayerController::FB_Move);
	InputComponent->BindAxis("LR_Move", this, &AMyPlayerController::LR_Move);
	InputComponent->BindAxis("Attack", this, &AMyPlayerController::Attack);

	InputComponent->BindAction("Jump", IE_Pressed, this, &AMyPlayerController::Jump);
}

void AMyPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	FHitResult HitResult;
	GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, HitResult);
	FRotator DiRo = (HitResult.Location - PlayerPawn->GetActorLocation()).Rotation();
	DiRo.Pitch = 0;
	DiRo.Roll = 0;

	PlayerPawn->SetRot(DiRo);
}