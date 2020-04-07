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
	~AMonsterActor();

	void SetData(FMonsterData _Data);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController);

	void StunEnd() { bStun = false; }
	void NuckBack();
	void NuckBackEnd() { GetWorldTimerManager().ClearTimer(NuckBackTimer); }

public:
	virtual void Damage(int _Damage, AActor * _ACKActor);

protected:
	AAIController* AiController;

	UStaticMesh* ProjectileMesh;

	int AttackSpeed = 0;
	int AttackDamage = 0;
	int DropMoney = 0;
	int Health = 0;

	bool bStun;

	FRotator DiRo;

	FTimerHandle StunTimer;
	FTimerHandle NuckBackTimer;
	FTimerHandle NuckBackEndTimer;
};
