// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss/BossActor.h"
#include "GolemBoss.generated.h"

/**
 * 
 */
UCLASS()
class ALEGENDARYMINE_API AGolemBoss : public ABossActor
{
	GENERATED_BODY()

public:
	AGolemBoss();

	void Attack();
	void AttackEnd();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void AttackTermFunc();
	void CanAttackFunc() { bCanAttack = true; }

	void AttackFunc();
	void AttackToGround();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
