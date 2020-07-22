// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "ALegendaryMine/Player/PlayerPawn.h"

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Adventurer.generated.h"

UCLASS()
class ALEGENDARYMINE_API AAdventurer : public APlayerPawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAdventurer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
