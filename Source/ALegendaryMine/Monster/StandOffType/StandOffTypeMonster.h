// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Monster/MonsterActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "StandOffTypeMonster.generated.h"

UCLASS()
class ALEGENDARYMINE_API AStandOffTypeMonster : public AMonsterActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AStandOffTypeMonster();

	UFUNCTION(BlueprintCallable)
		void Attack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void IsNotStun();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
