// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataTable/TemplateDataTable.h"
#include "DataTable/RoadTemplateDataTable.h"
#include "DataTable/MonsterDataTable.h"
#include "DataTable/WeaponDataTable.h"
#include "DataTable/CardDataTable.h"

#include "Struct/MapGeneratorStruct.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 *
 */
UCLASS()
class ALEGENDARYMINE_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();

protected:
	virtual void Init();

protected:
	TArray<class UDataTable*> TemplateDataTable;
	TArray<class UDataTable*> RoadTemplateDataTable;
	TArray<class UDataTable*> MonsterDataTable;
	
	class UDataTable* StageSizeDataTable;
	class UDataTable* WeaponDataTable;

	class UDataTable* BuffDataTable;
	class UDataTable* DeBuffDataTable;

public:
	TArray<TArray<FTemplateDataTableRow>> RoomTemplateData;
	TArray<TArray<FRoadMeshData>> RoadTemplateData;
	TArray<TArray<FMonsterDataTableRow>> MonsterData;

	TArray<FStageSizeDataTableRow> StageSizeData;
	TArray<FWeaponDataTableRow> WeaponData;

	TArray<FCardDataTableRow> BuffData;
	TArray<FCardDataTableRow> DeBuffData;
};
