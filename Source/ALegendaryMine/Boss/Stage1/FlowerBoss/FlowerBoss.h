// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Boss/BossActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FlowerBoss.generated.h"

UCLASS()
class ALEGENDARYMINE_API AFlowerBoss : public ABossActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFlowerBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
