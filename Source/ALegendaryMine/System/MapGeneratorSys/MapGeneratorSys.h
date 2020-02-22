// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ALegendaryMine/Struct/MapGeneratorStruct.h"

#include "CoreMinimal.h"

/**
 * 
 */
class ALEGENDARYMINE_API MapGeneratorSys
{
public:
	MapGeneratorSys();
	~MapGeneratorSys();

	void MapGen(int _Roomsize);
	TArray<FRoomData> GetRoomArray();
	TArray<FRoadData> GetRoadArray();
	FALGraph GetGraph();

protected:
	bool IntersectLine(const FVector & _SP1, const FVector & _EP1, const FVector & _SP2, const FVector & _EP2);

protected:
	FALGraph graph;

	TArray<FRoomData> RoomArray;
	TArray<FRoadData> RoadArray;
};
