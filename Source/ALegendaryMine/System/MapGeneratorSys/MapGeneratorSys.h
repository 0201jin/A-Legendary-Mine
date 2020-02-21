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
	bool GetIntersectPoint(const FVector& AP1, const FVector& AP2, const FVector& BP1, const FVector& BP2);

protected:
	FALGraph graph;

	TArray<FRoomData> RoomArray;
	TArray<FRoadData> RoadArray;
};
