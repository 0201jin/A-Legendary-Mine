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

	void MapGen();
	TArray<FRoomData> GetRoomArray();
	FALGraph GetGraph();

protected:
	FALGraph graph;

	TArray<FRoomData> RoomArray;
};
