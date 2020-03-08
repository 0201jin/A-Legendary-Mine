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

	Mapgen->MapGen(18);

	GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(FVector(0, 0, 100));

	TArray<FRoomData> RoomArray = Mapgen->GetRoomArray();

	for (int i = 0; i < RoomArray.Num(); i++)
	{
		DrawDebugBox(GetWorld(), FVector(RoomArray[i].X + (RoomArray[i].SX / 2), RoomArray[i].Y + (RoomArray[i].SY / 2), 0), FVector(RoomArray[i].SX / 2, RoomArray[i].SY / 2, 100), FColor::Blue, true, -1, 0, 20);
	}

	FALGraph graph = Mapgen->GetGraph();
	FPQueue copyPQ = graph.Queue;
	FGraphEdge edge;

	while (!PQIsEmpty(&copyPQ))
	{
		edge = PDequeue(&copyPQ);

		FVector Start = FVector(RoomArray[edge.Vertex1].X + (RoomArray[edge.Vertex1].SX / 2), RoomArray[edge.Vertex1].Y + (RoomArray[edge.Vertex1].SY / 2), 0);
		FVector End = FVector(RoomArray[edge.Vertex2].X + (RoomArray[edge.Vertex2].SX / 2), RoomArray[edge.Vertex2].Y + (RoomArray[edge.Vertex2].SY / 2), 0);

		DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, -1, 0, 50);
	}

	TArray<FRoadData> RoadArray = Mapgen->GetRoadArray();
	for (int fiCount = 0; fiCount < 2; fiCount++)
		for (int i = 0; i < RoadArray.Num(); i++)
		{
			DrawDebugLine(GetWorld(),
				FVector(RoadArray[i].Data[fiCount].X, RoadArray[i].Data[fiCount].Y, 0),
				FVector(RoadArray[i].Data[fiCount].X, RoadArray[i].Data[fiCount].Y, 0), FColor::Cyan, true, -1, 0, 100);

			DrawDebugLine(GetWorld(),
				FVector(RoadArray[i].Data[fiCount].X, RoadArray[i].Data[fiCount].Y, 0),
				RoadArray[i].Data[fiCount].V1R, FColor::Green, true, -1, 0, 10);

			DrawDebugLine(GetWorld(),
				FVector(RoadArray[i].Data[fiCount].X, RoadArray[i].Data[fiCount].Y, 0),
				RoadArray[i].Data[fiCount].V2R, FColor::Green, true, -1, 0, 10);
		}
}

void AInGame::GenerateMap()
{
	UE_LOG(LogTemp, Log, TEXT("------------------------------"));
	UE_LOG(LogTemp, Log, TEXT("------------------------------"));

	FlushPersistentDebugLines(GetWorld());

	Mapgen->DeleteMap();

	Mapgen->MapGen(18);

	TArray<FRoomData> RoomArray = Mapgen->GetRoomArray();

	for (int i = 0; i < RoomArray.Num(); i++)
	{
		DrawDebugBox(GetWorld(), FVector(RoomArray[i].X + (RoomArray[i].SX / 2), RoomArray[i].Y + (RoomArray[i].SY / 2), 0), FVector(RoomArray[i].SX / 2, RoomArray[i].SY / 2, 100), FColor::Blue, true, -1, 0, 20);
	}

	FALGraph graph = Mapgen->GetGraph();
	FPQueue copyPQ = graph.Queue;
	FGraphEdge edge;

	while (!PQIsEmpty(&copyPQ))
	{
		edge = PDequeue(&copyPQ);

		FVector Start = FVector(RoomArray[edge.Vertex1].X + (RoomArray[edge.Vertex1].SX / 2), RoomArray[edge.Vertex1].Y + (RoomArray[edge.Vertex1].SY / 2), 0);
		FVector End = FVector(RoomArray[edge.Vertex2].X + (RoomArray[edge.Vertex2].SX / 2), RoomArray[edge.Vertex2].Y + (RoomArray[edge.Vertex2].SY / 2), 0);

		DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, -1, 0, 50);
	}

	TArray<FRoadData> RoadArray = Mapgen->GetRoadArray();
	for (int fiCount = 0; fiCount < 2; fiCount++)
		for (int i = 0; i < RoadArray.Num(); i++)
		{
			DrawDebugLine(GetWorld(),
				FVector(RoadArray[i].Data[fiCount].X, RoadArray[i].Data[fiCount].Y, 0),
				FVector(RoadArray[i].Data[fiCount].X, RoadArray[i].Data[fiCount].Y, 0), FColor::Cyan, true, -1, 0, 100);

			DrawDebugLine(GetWorld(),
				FVector(RoadArray[i].Data[fiCount].X, RoadArray[i].Data[fiCount].Y, 0),
				RoadArray[i].Data[fiCount].V1R, FColor::Green, true, -1, 0, 10);

			DrawDebugLine(GetWorld(),
				FVector(RoadArray[i].Data[fiCount].X, RoadArray[i].Data[fiCount].Y, 0),
				RoadArray[i].Data[fiCount].V2R, FColor::Green, true, -1, 0, 10);
		}
}