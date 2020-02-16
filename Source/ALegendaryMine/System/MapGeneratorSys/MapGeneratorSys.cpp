// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGeneratorSys.h"

MapGeneratorSys::MapGeneratorSys()
{
}

MapGeneratorSys::~MapGeneratorSys()
{
}

void MapGeneratorSys::MapGen()
{
	for (int i = 0; i < 6; i++)
	{
		FRoomData Data;

		Data.X = 0;
		Data.Y = 0;
		Data.SX = ((rand() % 30) + 10) * 100;
		Data.SY = ((rand() % 30) + 10) * 100;

		RoomArray.Add(Data);
	}

	for (int i = 0; i < RoomArray.Num(); i++)
	{
		while (true)
		{
			bool Check = true;

			for (int x = 0; x < RoomArray.Num(); x++)
			{
				if (i != x)
				{
					int MinSX = RoomArray[i].SX;
					int MinSY = RoomArray[i].SY;

					if (RoomArray[i].X > RoomArray[x].X)
						MinSX = RoomArray[x].SX;
					if(RoomArray[i].Y > RoomArray[x].Y)
						MinSY = RoomArray[x].SY;

					if (abs(RoomArray[i].X - RoomArray[x].X) <= MinSX &&
						abs(RoomArray[i].Y - RoomArray[x].Y) <= MinSY) //충돌 범위 설정
					{
						RoomArray[i].X += ((rand() % 30) - 15) * 100;
						RoomArray[i].Y += ((rand() % 30) - 15) * 100;

						Check = false;
						break;
					}
				}
			}

			if (Check)
			{
				UE_LOG(LogTemp, Log, TEXT("%d"), i);
				break;
			}
		}
	}

	GraphInit(&graph, 6);
	
	for (int i = 0; i < RoomArray.Num(); i++)
	{
		for (int x = i; x < RoomArray.Num(); x++)
		{
			if (i != x)
			{
				float dis = FVector::Dist(FVector(RoomArray[i].X, RoomArray[i].Y, 0), FVector(RoomArray[x].X, RoomArray[x].Y, 0));

				AddEdge(&graph, i, x, (int)dis);
			}
		}
	}

	ConKruskalMST(&graph);
	ShowGraphEdgeInfo(&graph);

	UE_LOG(LogTemp, Log, TEXT("END"));
}

TArray<FRoomData> MapGeneratorSys::GetRoomArray()
{
	return RoomArray;
}

FALGraph MapGeneratorSys::GetGraph()
{
	return graph;
}
