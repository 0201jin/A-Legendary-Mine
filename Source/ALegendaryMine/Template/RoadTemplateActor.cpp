// Fill out your copyright notice in the Description page of Project Settings.

#include "RoadTemplateActor.h"

#define WR 2
#define WL 0
#define WF 1
#define WB 3

// Sets default values
ARoadTemplateActor::ARoadTemplateActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Floor = NewObject<UInstancedStaticMeshComponent>(this, TEXT("Floor"));
	Floor->AttachTo(RootComponent);

	Walls = NewObject<UInstancedStaticMeshComponent>(this, TEXT("Walls"));
	Walls->AttachTo(RootComponent);

	Door = NewObject<UInstancedStaticMeshComponent>(this, TEXT("Doors"));
	Door->AttachTo(RootComponent);
}

void ARoadTemplateActor::SetRoadMeshData(FRoadMeshData _MeshData, FRoadData _RoadData)
{
	MeshData = _MeshData;
	RoadData = _RoadData;

	Floor->SetStaticMesh(_MeshData.Floor);
	Walls->SetStaticMesh(_MeshData.Walls);
	Door->SetStaticMesh(_MeshData.Doors);

	FVector Road = FVector(RoadData.X, RoadData.Y, 0);

	FVector FloorVector = FVector(RoadData.X, RoadData.Y, -100);
	Floor->AddInstanceWorldSpace(FTransform(FloorVector));

	bool bWalls[4] = {false, false, false, false};

	if (RoadData.X == RoadData.V1RF.X)
	{
		if (RoadData.Y < RoadData.V1RF.Y)
		{
			bWalls[WR] = true;
		}
		else
		{
			bWalls[WL] = true;
		}
	}
	else if (RoadData.Y == RoadData.V1RF.Y)
	{
		if (RoadData.X < RoadData.V1RF.X)
		{
			bWalls[WF] = true;
		}
		else
		{
			bWalls[WB] = true;
		}
	}

	if (RoadData.X == RoadData.V2RF.X)
	{
		if (RoadData.Y < RoadData.V2RF.Y)
		{
			bWalls[WR] = true;
		}
		else
		{
			bWalls[WL] = true;
		}
	}
	else if (RoadData.Y == RoadData.V2RF.Y)
	{
		if (RoadData.X < RoadData.V2RF.X)
		{
			bWalls[WF] = true;
		}
		else
		{
			bWalls[WB] = true;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		FVector Lo = Road;

		switch (i)
		{
		case WL:
			Lo.Y -= 100;
			break;

		case WF:
			Lo.X += 100;
			break;

		case WR:
			Lo.Y += 100;
			break;
		
		case WB:
			Lo.X -= 100;
			break;
		}

		if (!bWalls[i])
		{
			Walls->AddInstanceWorldSpace(FTransform(FRotator(0, 90 * i, 0), Lo, FVector(1, 1, 1)));
			UE_LOG(LogTemp, Log, TEXT("%s"), *GetName());
		}
	}

	CreateRoad(Road, Road + (RoadData.V1RF - RoadData.V1R));
	CreateRoad(Road, Road + (RoadData.V2RF - RoadData.V2R));
}

// Called when the game starts or when spawned
void ARoadTemplateActor::BeginPlay()
{
	Super::BeginPlay();

}

void ARoadTemplateActor::CreateRoad(FVector _Road, FVector _Lo)
{
	if (_Road == RoadData.V1RF)
	{
		if (_Road.X == _Lo.X)
		{
			Door->AddInstanceWorldSpace(FTransform(FRotator(0, 0, 0), _Road, FVector(1, 1, 1)));
		}
		else if (_Road.Y == _Lo.Y)
		{
			Door->AddInstanceWorldSpace(FTransform(FRotator(0, 90, 0), _Road, FVector(1, 1, 1)));
		}

		return;
	}
	else if (_Road == RoadData.V2RF)
	{
		if (_Road.X == _Lo.X)
		{
			Door->AddInstanceWorldSpace(FTransform(FRotator(0, 0, 0), _Road, FVector(1, 1, 1)));
		}
		else if (_Road.Y == _Lo.Y)
		{
			Door->AddInstanceWorldSpace(FTransform(FRotator(0, 90, 0), _Road, FVector(1, 1, 1)));
		}

		return;
	}

	//_Lo에 길을 만든다.
	FVector FloorVector = _Lo;
	FloorVector.Z = -100;
	Floor->AddInstanceWorldSpace(FTransform(FloorVector));

	
	if (_Road.X == _Lo.X)
	{
		FVector Lo = _Lo;
		FVector Lo1 = _Lo;
		Lo.X += 100;
		Lo1.X -= 100;

		Walls->AddInstanceWorldSpace(FTransform(FRotator(0, 90 * WF, 0), Lo, FVector(1, 1, 1)));
		Walls->AddInstanceWorldSpace(FTransform(FRotator(0, 90 * WB, 0), Lo1, FVector(1, 1, 1)));
	}
	else if (_Road.Y == _Lo.Y)
	{
		FVector Lo = _Lo;
		FVector Lo1 = _Lo;
		Lo.Y += 100;
		Lo1.Y -= 100;

		Walls->AddInstanceWorldSpace(FTransform(FRotator(0, 90 * WR, 0), Lo, FVector(1, 1, 1)));
		Walls->AddInstanceWorldSpace(FTransform(FRotator(0, 90 * WL, 0), Lo1, FVector(1, 1, 1)));
	}

	if (_Road.X == _Lo.X)
	{
		CreateRoad(_Lo, FVector(_Lo.X, _Lo.Y + (_Lo.Y - _Road.Y), 0));
	}
	else if (_Road.Y == _Lo.Y)
	{
		CreateRoad(_Lo, FVector(_Lo.X + (_Lo.X - _Road.X), _Lo.Y, 0));
	}
}
