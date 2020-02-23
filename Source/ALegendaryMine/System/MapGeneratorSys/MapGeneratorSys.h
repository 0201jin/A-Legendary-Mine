// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ALegendaryMine/Struct/MapGeneratorStruct.h"
#include "MyCustomAsset.h"

#include "CoreMinimal.h"

/**
 * 
 */
class ALEGENDARYMINE_API MapGeneratorSys
{
public:
	MapGeneratorSys(class AInGame * _InGameLevel);
	~MapGeneratorSys();

	void MapGen(int _Roomsize);
	TArray<FRoomData> GetRoomArray();
	TArray<FRoadData> GetRoadArray();
	FALGraph GetGraph();

protected:
	bool IntersectLine(const FVector & _SP1, const FVector & _EP1, const FVector & _SP2, const FVector & _EP2);

protected:
	FALGraph graph;

	class AInGame * InGameLevel;
	class UMyGameInstance * GameInstance;

	TArray<FRoomData> RoomArray;
	TArray<FRoadData> RoadArray;

	TArray<TArray<UMyCustomAsset*>> TemplateArray;
	TArray<class ARoomTemplateActor*> TemplateActorArray;
};
