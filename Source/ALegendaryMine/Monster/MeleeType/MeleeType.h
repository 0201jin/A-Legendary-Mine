// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Monster/MonsterActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MeleeType.generated.h"

UCLASS()
class ALEGENDARYMINE_API AMeleeType : public AMonsterActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMeleeType();

	UFUNCTION(BlueprintCallable)
		void Attack();

	UFUNCTION(BlueprintCallable)
		void EndAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void IsNotStun();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
