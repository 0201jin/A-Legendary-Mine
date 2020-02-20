// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGeneratorSys.h"

MapGeneratorSys::MapGeneratorSys()
{
}

MapGeneratorSys::~MapGeneratorSys()
{
}

void MapGeneratorSys::MapGen(int _Roomsize)
{
	for (int i = 0; i < _Roomsize; i++)
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
					if (RoomArray[i].Y > RoomArray[x].Y)
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

	GraphInit(&graph, _Roomsize);

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

	/*엣지를 중심으로 길의 정점을 만드는 코드*/
	FPQueue copyPQ = graph.Queue;
	FGraphEdge edge;

	while (!PQIsEmpty(&copyPQ))
	{
		edge = PDequeue(&copyPQ);

		FRoomData Data;

		if (RoomArray[edge.Vertex1].X > RoomArray[edge.Vertex2].X)
		{
			Data.X = RoomArray[edge.Vertex2].X;
			Data.SX = (RoomArray[edge.Vertex1].X + RoomArray[edge.Vertex1].SX) - RoomArray[edge.Vertex2].X;
		}
		else
		{
			Data.X = RoomArray[edge.Vertex1].X;
			Data.SX = (RoomArray[edge.Vertex2].X + RoomArray[edge.Vertex2].SX) - RoomArray[edge.Vertex1].X;
		}

		if (RoomArray[edge.Vertex1].Y > RoomArray[edge.Vertex2].Y)
		{
			Data.Y = RoomArray[edge.Vertex2].Y;
			Data.SY = (RoomArray[edge.Vertex1].Y + RoomArray[edge.Vertex1].SY) - RoomArray[edge.Vertex2].Y;
		}
		else
		{
			Data.Y = RoomArray[edge.Vertex1].Y;
			Data.SY = (RoomArray[edge.Vertex2].Y + RoomArray[edge.Vertex2].SY) - RoomArray[edge.Vertex1].Y;
		}

		while (true)
		{
			/*
			랜덤으로 뽑아서 길목 조건에 충족하는지 확인
			v1.x & v2.y || v1.y & v2.x || v1.x & v2.x || v1.y & v2.y
			*/

			bool bCheck = false;

			int x = ((rand() % Data.SX) + Data.X) * 0.01;
			int y = ((rand() % Data.SY) + Data.Y) * 0.01;

			x *= 100;
			y *= 100;

			if ((RoomArray[edge.Vertex1].X < x && RoomArray[edge.Vertex1].X + RoomArray[edge.Vertex1].SX > x &&
				 RoomArray[edge.Vertex2].Y < y && RoomArray[edge.Vertex2].Y + RoomArray[edge.Vertex2].SY > y) ||
				(RoomArray[edge.Vertex1].Y < y && RoomArray[edge.Vertex1].Y + RoomArray[edge.Vertex1].SY > y &&
				 RoomArray[edge.Vertex2].X < x && RoomArray[edge.Vertex2].X + RoomArray[edge.Vertex2].SX > x) ||
				(RoomArray[edge.Vertex1].X < x && RoomArray[edge.Vertex1].X + RoomArray[edge.Vertex1].SX > x &&
				 RoomArray[edge.Vertex2].X < x && RoomArray[edge.Vertex2].X + RoomArray[edge.Vertex2].SX > x) ||
				(RoomArray[edge.Vertex1].Y < y && RoomArray[edge.Vertex1].Y + RoomArray[edge.Vertex1].SY > y &&
				 RoomArray[edge.Vertex2].Y < y && RoomArray[edge.Vertex2].Y + RoomArray[edge.Vertex2].SY > y))
			{
				bCheck = true;

				for (int i = 0; i < RoomArray.Num(); i++)
				{
					if (((RoomArray[i].X <= x && RoomArray[i].X + RoomArray[i].SX >= x) &&
						(RoomArray[i].Y <= y && RoomArray[i].Y + RoomArray[i].SY >= y))) //충돌 범위 설정
					{
						bCheck = false;
						break;
					}
				}
			}

			if (bCheck)
			{
				FRoadData Data;
				Data.V1 = edge.Vertex1;
				Data.V2 = edge.Vertex2;
				Data.X = x;
				Data.Y = y;

				RoadArray.Add(Data);

				break;
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("END"));
}

TArray<FRoomData> MapGeneratorSys::GetRoomArray()
{
	return RoomArray;
}

TArray<FRoadData> MapGeneratorSys::GetRoadArray()
{
	return RoadArray;
}

FALGraph MapGeneratorSys::GetGraph()
{
	return graph;
}
