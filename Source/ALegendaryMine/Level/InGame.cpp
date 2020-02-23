// Fill out your copyright notice in the Description page of Project Settings.

#include "InGame.h"
#include "DrawDebugHelpers.h"

AInGame::AInGame()
{
}

AInGame::~AInGame()
{
}

UMyGameInstance * AInGame::GetMyGameInstance()
{
	return MyGameInstance;
}

void AInGame::BeginPlay()
{
	Super::BeginPlay();

	MyGameInstance = Cast<UMyGameInstance>(GetGameInstance());

	if (Mapgen != nullptr)
	{
		delete(Mapgen);
	}

	Mapgen = new MapGeneratorSys(this);

	Mapgen->MapGen(30);
	TArray<FRoomData> RoomArray = Mapgen->GetRoomArray();

	for (int i = 0; i < RoomArray.Num(); i++)
	{
		DrawDebugBox(GetWorld(), FVector(RoomArray[i].X + (RoomArray[i].SX / 2), RoomArray[i].Y + (RoomArray[i].SY / 2), 0), FVector(RoomArray[i].SX / 2, RoomArray[i].SY / 2, 100), FColor::Blue, true, -1, 0, 20);
	}

	TArray<FRoadData> RoadArray = Mapgen->GetRoadArray();
	for (int i = 0; i < RoadArray.Num(); i++)
	{
		DrawDebugLine(GetWorld(),
			FVector(RoadArray[i].X, RoadArray[i].Y, 0),
			FVector(RoadArray[i].X, RoadArray[i].Y, 0), FColor::Cyan, true, -1, 0, 100);

		DrawDebugLine(GetWorld(),
			FVector(RoadArray[i].X, RoadArray[i].Y, 0),
			RoadArray[i].V1R, FColor::Green, true, -1, 0, 100);

		DrawDebugLine(GetWorld(),
			FVector(RoadArray[i].X, RoadArray[i].Y, 0),
			RoadArray[i].V2R, FColor::Green, true, -1, 0, 100);
	}
}
