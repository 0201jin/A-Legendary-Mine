// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DataTable/CardDataTable.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "CardActor.h"

#include "CoreMinimal.h"
#include "CardSystem.generated.h"

/**
 * 
 */
UCLASS()
class ALEGENDARYMINE_API ACardSystem : public AActor
{
	GENERATED_BODY()

public:
	ACardSystem();
	~ACardSystem();

	void RotBAll();
	void MoveAll();
	void RotFAll();
	void MoveToField();

	void SpawnCard();
	void DestroyCard();

	void InitCard();
	void AddCard(FCardDataTableRow _Buff, FCardDataTableRow _DeBuff);

	void SpawnType(FCardDataTableRow _Data);
	void TickType(FCardDataTableRow _Data);
	void AddDamageType(FCardDataTableRow _Data);
	void StatType(FCardDataTableRow _Data);
	void AvoidType(FCardDataTableRow _Data);

protected:
	void SpawnObject(UStaticMesh * _Mesh);

protected:
	TArray<FTimerHandle> TimeHandlerArray;
	TArray<FTimerDelegate> DelegateArray;
	TArray<FCardDataTableRow> CardArray;

	TArray<ACardActor*> CardActorArray;

	FTimerHandle MoveTimer;
	int RotCounter;

	class AInGame * LevelScript;
};
