// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "Engine.h"
#include "Struct/MapGeneratorStruct.h"
#include "MyGameInstance.h"
#include "Monster/MonsterActor.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossActor.generated.h"

UCLASS()
class ALEGENDARYMINE_API ABossActor : public AMonsterActor
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController);

	virtual void Dead();

	void InitSuddenAttack() { bSuddenAttack = true; }

public:	
	virtual void Damage(int _Damage, AActor* _ACKActor);

	UFUNCTION(BlueprintCallable)
		virtual void Attack();

	UFUNCTION(BlueprintCallable)
		virtual void AttackEnd();

	UFUNCTION(BlueprintCallable)
		virtual void Pause();

	UFUNCTION(BlueprintCallable)
		virtual void End();

protected:
	AAIController* AiController;

	APawn* Player;

	FTimerHandle AttackTerm;
	FTimerHandle AttackTimer;
	FTimerHandle SuddenAttackTimer;

	UAnimInstance* AnimInstance;

	TArray<UAnimMontage*> AnimArray;
	UAnimMontage* DeadAnim;

	class AHitBoxMTP* HitBox;

	int Health;
	int AttackParam = 0;

	bool bDead = false;
	bool bCanAttack = false;
	bool bSuddenAttack = true;
};
