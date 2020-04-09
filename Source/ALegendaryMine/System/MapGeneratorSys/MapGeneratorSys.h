// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ALegendaryMine/Struct/MapGeneratorStruct.h"
#include "MyCustomAsset.h"
#include "Template/RoomDoor.h"
#include "InteractionActor/RoomActiveActor.h"
#include "Monster/MonsterActor.h"
#include "Monster/BumpType/BumpTypeMonster.h"
#include "Monster/MeleeType/MeleeType.h"

#include "CoreMinimal.h"

/**
 * 
 */
class ALEGENDARYMINE_API MapGeneratorSys
{
public:
	MapGeneratorSys(class AInGame * _InGameLevel);
	~MapGeneratorSys();

	void MapGen(int _Stage);
	void DeleteMap();

	void RoomActiveActor(int _RoomNumber);
	void RoomInActiveActor();
	void DestroyMonster();

	TArray<FRoomData> GetRoomArray();
	TArray<FRoadData> GetRoadArray();
	TArray<ARoomDoor*> GetDoorArray();

	FALGraph GetGraph();

	FVector GetStartLo();

	int GetMaxDisIndex();

protected:
	bool IntersectLine(const FVector & _SP1, const FVector & _EP1, const FVector & _SP2, const FVector & _EP2);

protected:
	FALGraph graph;

	class AInGame * InGameLevel;
	class UMyGameInstance * GameInstance;

	TArray<FRoomData> RoomArray;
	TArray<FRoadData> RoadArray;
	TArray<TArray<FMonsterDataTableRow>> MonsterArray;

	TArray<class ARoomTemplateActor*> TemplateActorArray;
	TArray<class ARoadTemplateActor*> RoadTemplateActorArray;
	TArray<ARoomDoor*> DoorArray;
	TArray<TArray<ARoomActiveActor*>> RoomActiveActorArray;

	int MaxDisIndex;
	int iStage = 0;
	int iMonsterCount = 0;
};
