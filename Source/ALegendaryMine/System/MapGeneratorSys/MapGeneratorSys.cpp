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

		int wi = 0;

		while (wi++ < 100)
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

			FRoadData Data;
			Data.V1 = edge.Vertex1;
			Data.V2 = edge.Vertex2;
			Data.X = x;
			Data.Y = y;

			if (RoomArray[Data.V1].X < Data.X &&
				RoomArray[Data.V1].X + RoomArray[Data.V1].SX > Data.X)
			{
				if (RoomArray[Data.V1].Y > Data.Y) //길을 오른쪽으로
				{
					Data.V1R = FVector(x, RoomArray[edge.Vertex1].Y - 1, 0);
				}
				else //길을 왼쪽으로
				{
					Data.V1R = FVector(x, RoomArray[edge.Vertex1].Y + RoomArray[edge.Vertex1].SY + 1, 0);
				}
			}
			else if (RoomArray[Data.V1].Y < Data.Y &&
				RoomArray[Data.V1].Y + RoomArray[Data.V1].SY > Data.Y)
			{
				if (RoomArray[Data.V1].X > Data.X) //길을 위로
				{
					Data.V1R = FVector(RoomArray[edge.Vertex1].X -1, y, 0);
				}
				else //길을 아래로
				{
					Data.V1R = FVector(RoomArray[edge.Vertex1].X + RoomArray[edge.Vertex1].SX +1, y, 0);
				}
			}

			if (RoomArray[Data.V2].X < Data.X &&
				RoomArray[Data.V2].X + RoomArray[Data.V2].SX > Data.X)
			{
				if (RoomArray[Data.V2].Y > Data.Y) //길을 오른쪽으로
				{
					Data.V2R = FVector(x, RoomArray[edge.Vertex2].Y -1, 0);
				}
				else //길을 왼쪽을 
				{
					Data.V2R = FVector(x, RoomArray[edge.Vertex2].Y + RoomArray[edge.Vertex2].SY +1, 0);
				}
			}
			else if (RoomArray[Data.V2].Y < Data.Y &&
				RoomArray[Data.V2].Y + RoomArray[Data.V2].SY > Data.Y)
			{
				if (RoomArray[Data.V2].X > Data.X) //길을 위로
				{
					Data.V2R = FVector(RoomArray[edge.Vertex2].X - 1, y, 0);
				}
				else //길을 아래로
				{
					Data.V2R = FVector(RoomArray[edge.Vertex2].X + RoomArray[edge.Vertex2].SX + 1, y, 0);
				}
			}

			/*충돌하는 길이 있는지 확인*/

			for (int i = 0; i < RoadArray.Num(); i++)
			{
				if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V1R,
					FVector(RoadArray[i].X, RoadArray[i].Y, 0), RoadArray[i].V1R))
				{
					bCheck = false;
					break;
				}
				if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V1R,
					FVector(RoadArray[i].X, RoadArray[i].Y, 0), RoadArray[i].V2R))
				{
					bCheck = false;
					break;
				}

				if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V2R,
					FVector(RoadArray[i].X, RoadArray[i].Y, 0), RoadArray[i].V1R))
				{
					bCheck = false;
					break;
				}
				if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V2R,
					FVector(RoadArray[i].X, RoadArray[i].Y, 0), RoadArray[i].V2R))
				{
					bCheck = false;
					break;
				}
			}

			/*충돌하는 방이 있는지 확인*/
			for (int i = 0; i < RoomArray.Num(); i++)
			{
				if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V1R,
					FVector(RoomArray[i].X, RoomArray[i].Y, 0),
					FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y, 0)))
				{
					bCheck = false;
					break;
				}
				if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V1R,
					FVector(RoomArray[i].X, RoomArray[i].Y, 0),
					FVector(RoomArray[i].X, RoomArray[i].Y + RoomArray[i].SY, 0)))
				{
					bCheck = false;
					break;
				}
				if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V1R,
					FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y, 0),
					FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y + RoomArray[i].SY, 0)))
				{
					bCheck = false;
					break;
				}
				if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V1R,
					FVector(RoomArray[i].X, RoomArray[i].Y + RoomArray[i].SY, 0),
					FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y + RoomArray[i].SY, 0)))
				{
					bCheck = false;
					break;
				}

				if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V2R,
					FVector(RoomArray[i].X, RoomArray[i].Y, 0),
					FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y, 0)))
				{
					bCheck = false;
					break;
				}
				if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V2R,
					FVector(RoomArray[i].X, RoomArray[i].Y, 0),
					FVector(RoomArray[i].X, RoomArray[i].Y + RoomArray[i].SY, 0)))
				{
					bCheck = false;
					break;
				}
				if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V2R,
					FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y, 0),
					FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y + RoomArray[i].SY, 0)))
				{
					bCheck = false;
					break;
				}
				if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V2R,
					FVector(RoomArray[i].X, RoomArray[i].Y + RoomArray[i].SY, 0),
					FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y + RoomArray[i].SY, 0)))
				{
					bCheck = false;
					break;
				}
			}

			if (bCheck)
			{
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

bool MapGeneratorSys::IntersectLine(const FVector & _SP1, const FVector & _EP1, const FVector & _SP2, const FVector & _EP2)
{
	float den = (_EP2.Y - _SP2.Y)*(_EP1.X - _SP1.X) - (_EP2.X - _SP2.X)*(_EP1.Y - _SP1.Y);
	if (den == 0) return false;

	float p1 = (_EP2.X - _SP2.X)*(_SP1.Y - _SP2.Y) - (_EP2.Y - _SP2.Y)*(_SP1.X - _SP2.X);
	float p2 = (_EP1.X - _SP1.X)*(_SP1.Y - _SP2.Y) - (_EP1.Y - _SP1.Y)*(_SP1.X - _SP2.X);

	float op1 = p1 / den;
	float op2 = p2 / den;

	if (op1 < 0.0 || op1 > 1.0 || op2 < 0.0 || op2 > 1.0) 
		return false;
	if (p1 == 0 && p2 == 0) 
		return false;

	return true;
}
