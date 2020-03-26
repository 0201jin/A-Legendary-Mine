// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "Engine.h"
#include "Struct/MapGeneratorStruct.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonsterActor.generated.h"

UCLASS()
class ALEGENDARYMINE_API AMonsterActor : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMonsterActor();

	void SetData(FMonsterData _Data);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController);

public:
	virtual void Damage(int _Damage);

protected:
	AAIController* AiController;

	UStaticMesh* ProjectileMesh;

	int AttackSpeed = 0;
	int AttackDamage = 0;
	int DropMoney = 0;
	int Health = 0;
};
