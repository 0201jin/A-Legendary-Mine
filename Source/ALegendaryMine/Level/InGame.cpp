// Fill out your copyright notice in the Description page of Project Settings.

#include "InGame.h"
#include "DrawDebugHelpers.h"

AInGame::AInGame()
{
	Mapgen = new MapGeneratorSys;
}

AInGame::~AInGame()
{
}

void AInGame::BeginPlay()
{
	Mapgen->MapGen();
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

		DrawDebugLine(GetWorld(), Start, End, FColor::Red, true, -1, 0, 10);
		UE_LOG(LogTemp, Log, TEXT("%d-%d weight : %d"), edge.Vertex1, edge.Vertex2, edge.Weight);
	}
}
