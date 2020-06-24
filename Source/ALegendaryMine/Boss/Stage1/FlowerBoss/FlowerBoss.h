// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Boss/BossActor.h"
#include "Engine.h"

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

	void Attack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void AttackToRoot();
	void AttackToSide();
	void AttackToSky();

	void PlayATS();

	void AttackTermFunc();
	void CanAttackFunc() { bCanAttack = true; }

	void Dead();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UAnimMontage* AttackToRootAnim;
	UAnimMontage* AttackToSideAnim;
	UAnimMontage* AttackToSkyAnim;
	UAnimMontage* DeadAnim;

	FTimerHandle AttackTerm;
	FTimerHandle AttackTimer;

	bool bCanAttack = false;
};
