// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "DataTable/CardDataTable.h"
#include "Engine.h"
#include "GameFramework/Actor.h"

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

	class AInGame * LevelScript;
};
