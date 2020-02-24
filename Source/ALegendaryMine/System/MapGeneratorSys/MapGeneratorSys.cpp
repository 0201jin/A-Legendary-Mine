// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGeneratorSys.h"
#include "Level/InGame.h"
#include "Template/RoomTemplateActor.h"

MapGeneratorSys::MapGeneratorSys(class AInGame * _InGameLevel)
{
	InGameLevel = _InGameLevel;
	GameInstance = InGameLevel->GetMyGameInstance();
}

MapGeneratorSys::~MapGeneratorSys()
{
}

void MapGeneratorSys::MapGen(int _Roomsize)
{
	for (int i = 0; i < _Roomsize; i++)
	{
		int RandomIndex = (rand() % GameInstance->RoomTemplateData[0].Num());

		FRoomData Data;

		Data.X = 0;
		Data.Y = 0;
		Data.SX = (GameInstance->RoomTemplateData[0][RandomIndex]->SX - 1) * 100;
		Data.SY = (GameInstance->RoomTemplateData[0][RandomIndex]->SY - 1) * 100;

		TemplateActorArray.Add(
			InGameLevel->GetWorld()->SpawnActor<ARoomTemplateActor>(
				ARoomTemplateActor::StaticClass(),
				FTransform(FRotator(0, 0, 0),
					FVector(Data.X, Data.Y, 0),
					FVector(1, 1, 1))));

		TemplateActorArray[i]->SetAsset(GameInstance->RoomTemplateData[0][RandomIndex]);

		RoomArray.Add(Data);
	}

	//방이 겹치지 않게 퍼트림
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

					if (abs(RoomArray[i].X - RoomArray[x].X) <= (MinSX + 1) &&
						abs(RoomArray[i].Y - RoomArray[x].Y) <= (MinSY + 1))
					{
						RoomArray[i].X += ((rand() % 31) - 15) * 100;
						RoomArray[i].Y += ((rand() % 31) - 15) * 100;

						TemplateActorArray[i]->SetActorLocation(FVector(RoomArray[i].X, RoomArray[i].Y, 0));

						Check = false;
						break;
					}
				}
			}

			if (Check)
			{
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
	FPQueue * copyPQ = &graph.Queue;
	FGraphEdge recvEdge[ARR_SIZE];
	int eidx = 0;
	FGraphEdge edge;
	bool CheckEdge[256][256] = { false };

	while (!PQIsEmpty(copyPQ))
	{
		edge = PDequeue(copyPQ);

		FRoomData Data;

		CheckEdge[edge.Vertex1][edge.Vertex2] = true;

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

		TArray<FVector> RoadLo;

		int wi = 0;

		int V1X = RoomArray[edge.Vertex1].X;
		int V1SX = RoomArray[edge.Vertex1].SX;
		int V1Y = RoomArray[edge.Vertex1].Y;
		int V1SY = RoomArray[edge.Vertex1].SY;

		int V2X = RoomArray[edge.Vertex2].X;
		int V2SX = RoomArray[edge.Vertex2].SX;
		int V2Y = RoomArray[edge.Vertex2].Y;
		int V2SY = RoomArray[edge.Vertex2].SY;

		for (int x = Data.X; x < Data.X + Data.SX;)
		{
			for (int y = Data.Y; y < Data.Y + Data.SY;)
			{
				if ((
					(V1X < x && V1X + V1SX > x && V2Y < y && V2Y + V2SY > y) ||
					(V1Y < y && V1Y + V1SY > y && V2X < x && V2X + V2SX > x) ||
					(V1X < x && V1X + V1SX > x && V2X < x && V2X + V2SX > x) ||
					(V1Y < y && V1Y + V1SY > y && V2Y < y && V2Y + V2SY > y)) &&
					!(V1X <= x && V1X + V1SX >= x && V1Y <= y && V1Y + V1SY >= y) &&
					!(V2X <= x && V2X + V2SX >= x && V2Y <= y && V2Y + V2SY >= y))
				{
					RoadLo.Add(FVector(x, y, 0));
				}
				y += 100;
			}
			x += 100;
		}

		bool bCreateRoad = false;

		if (RoadLo.Num() > 0)
			while (wi++ < RoadLo.Num())
			{
				/*
				랜덤으로 뽑아서 길목 조건에 충족하는지 확인
				v1.x & v2.y || v1.y & v2.x || v1.x & v2.x || v1.y & v2.y
				*/

				int Why = 0;
				bool bCheck = true;
				int RoadIndex = rand() % RoadLo.Num();

				int x = RoadLo[RoadIndex].X;
				int y = RoadLo[RoadIndex].Y;

				for (int i = 0; i < RoomArray.Num(); i++)
				{
					if (((RoomArray[i].X <= x && RoomArray[i].X + RoomArray[i].SX >= x) &&
						(RoomArray[i].Y <= y && RoomArray[i].Y + RoomArray[i].SY >= y))) //충돌 범위 설정
					{
						Why = 1;
						bCheck = false;
						break;
					}
				}

				/*겹치는 길목 체크*/
				for (int i = 0; i < RoadArray.Num(); i++)
				{
					if (RoadArray[i].X == x && RoadArray[i].Y && y)
					{
						bCheck = false;
						break;
					}
				}

				FRoadData Data;
				Data.V1 = edge.Vertex1;
				Data.V2 = edge.Vertex2;
				Data.X = x;
				Data.Y = y;

				bool ShortRoadV1 = false;
				bool ShortRoadV2 = false;

				if (RoomArray[Data.V1].X < Data.X &&
					RoomArray[Data.V1].X + RoomArray[Data.V1].SX > Data.X)
				{
					if (RoomArray[Data.V1].Y > Data.Y) //길을 오른쪽으로
					{
						Data.V1R = FVector(x, RoomArray[edge.Vertex1].Y - 100, 0);
					}
					else //길을 왼쪽으로
					{
						Data.V1R = FVector(x, RoomArray[edge.Vertex1].Y + RoomArray[edge.Vertex1].SY + 100, 0);
					}

					if (int(Data.V1R.Y) == Data.Y)
						ShortRoadV1 = true;
				}
				else if (RoomArray[Data.V1].Y < Data.Y &&
					RoomArray[Data.V1].Y + RoomArray[Data.V1].SY > Data.Y)
				{
					if (RoomArray[Data.V1].X > Data.X) //길을 위로
					{
						Data.V1R = FVector(RoomArray[edge.Vertex1].X - 100, y, 0);
					}
					else //길을 아래로
					{
						Data.V1R = FVector(RoomArray[edge.Vertex1].X + RoomArray[edge.Vertex1].SX + 100, y, 0);
					}

					if (int(Data.V1R.X) == Data.X)
						ShortRoadV1 = true;
				}

				if (RoomArray[Data.V2].X < Data.X &&
					RoomArray[Data.V2].X + RoomArray[Data.V2].SX > Data.X)
				{
					if (RoomArray[Data.V2].Y > Data.Y) //길을 오른쪽으로
					{
						Data.V2R = FVector(x, RoomArray[edge.Vertex2].Y - 100, 0);
					}
					else //길을 왼쪽을 
					{
						Data.V2R = FVector(x, RoomArray[edge.Vertex2].Y + RoomArray[edge.Vertex2].SY + 100, 0);
					}

					if (int(Data.V2R.Y) == Data.Y)
						ShortRoadV2 = true;
				}
				else if (RoomArray[Data.V2].Y < Data.Y &&
					RoomArray[Data.V2].Y + RoomArray[Data.V2].SY > Data.Y)
				{
					if (RoomArray[Data.V2].X > Data.X) //길을 위로
					{
						Data.V2R = FVector(RoomArray[edge.Vertex2].X - 100, y, 0);
					}
					else //길을 아래로
					{
						Data.V2R = FVector(RoomArray[edge.Vertex2].X + RoomArray[edge.Vertex2].SX + 100, y, 0);
					}

					if (int(Data.V2R.X) == Data.X)
						ShortRoadV2 = true;
				}

				/*충돌하는 길이 있는지 확인*/
				if (bCheck)
				{
					for (int i = 0; i < RoadArray.Num(); i++)
					{
						if (!ShortRoadV1)
						{
							if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V1R,
								FVector(RoadArray[i].X, RoadArray[i].Y, 0), RoadArray[i].V1R))
							{
								Why = 2;
								bCheck = false;
								break;
							}
							if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V1R,
								FVector(RoadArray[i].X, RoadArray[i].Y, 0), RoadArray[i].V2R))
							{
								Why = 2;
								bCheck = false;
								break;
							}
						}

						if (!ShortRoadV2)
						{
							if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V2R,
								FVector(RoadArray[i].X, RoadArray[i].Y, 0), RoadArray[i].V1R))
							{
								Why = 2;
								bCheck = false;
								break;
							}
							if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V2R,
								FVector(RoadArray[i].X, RoadArray[i].Y, 0), RoadArray[i].V2R))
							{
								Why = 2;
								bCheck = false;
								break;
							}
						}
					}
				}

				/*충돌하는 길목이 있는지 확인*/
				if (bCheck)
				{
					for (int i = 0; i < RoadArray.Num(); i++)
					{
						if (!ShortRoadV1)
						{
							int MinX = Data.X;
							int MinY = Data.Y;
							int XABS = abs(MinX - RoadArray[i].V1R.X);
							int YABS = abs(MinY - RoadArray[i].V1R.Y);

							if (Data.X > RoadArray[i].V1R.X)
								MinX = RoadArray[i].V1R.X;
							if (Data.Y > RoadArray[i].V1R.Y)
								MinY = RoadArray[i].V1R.Y;

							for (int j = 0; j < XABS; j++)
							{
								if (MinX + j == Data.X && RoadArray[i].V1R.Y == Data.Y)
								{
									bCheck = false;
									break;
								}
							}

							for (int j = 0; j < YABS; j++)
							{
								if (MinY + j == Data.Y && RoadArray[i].V1R.X == Data.X)
								{
									bCheck = false;
									break;
								}
							}
						}

						if (!ShortRoadV2)
						{
							int MinX = Data.X;
							int MinY = Data.Y;
							int XABS = abs(MinX - RoadArray[i].V2R.X);
							int YABS = abs(MinY - RoadArray[i].V2R.Y);

							if (Data.X > RoadArray[i].V2R.X)
								MinX = RoadArray[i].V2R.X;
							if (Data.Y > RoadArray[i].V2R.Y)
								MinY = RoadArray[i].V2R.Y;

							for (int j = 0; j < XABS; j++)
							{
								if (MinX + j == Data.X && RoadArray[i].V2R.Y == Data.Y)
								{
									bCheck = false;
									break;
								}
							}

							for (int j = 0; j < YABS; j++)
							{
								if (MinY + j == Data.Y && RoadArray[i].V2R.X == Data.X)
								{
									bCheck = false;
									break;
								}
							}
						}
					}
				}

				/*충돌하는 방이 있는지 확인*/

				if (bCheck)
				{
					for (int i = 0; i < RoomArray.Num(); i++)
					{
						if (!ShortRoadV1)
						{
							if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V1R,
								FVector(RoomArray[i].X, RoomArray[i].Y, 0),
								FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y, 0)))
							{
								Why = 3;
								bCheck = false;
								break;
							}
							if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V1R,
								FVector(RoomArray[i].X, RoomArray[i].Y, 0),
								FVector(RoomArray[i].X, RoomArray[i].Y + RoomArray[i].SY, 0)))
							{
								Why = 3;
								bCheck = false;
								break;
							}
							if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V1R,
								FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y, 0),
								FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y + RoomArray[i].SY, 0)))
							{
								Why = 3;
								bCheck = false;
								break;
							}
							if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V1R,
								FVector(RoomArray[i].X, RoomArray[i].Y + RoomArray[i].SY, 0),
								FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y + RoomArray[i].SY, 0)))
							{
								Why = 3;
								bCheck = false;
								break;
							}
						}

						if (!ShortRoadV2)
						{
							if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V2R,
								FVector(RoomArray[i].X, RoomArray[i].Y, 0),
								FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y, 0)))
							{
								Why = 3;
								bCheck = false;
								break;
							}
							if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V2R,
								FVector(RoomArray[i].X, RoomArray[i].Y, 0),
								FVector(RoomArray[i].X, RoomArray[i].Y + RoomArray[i].SY, 0)))
							{
								Why = 3;
								bCheck = false;
								break;
							}
							if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V2R,
								FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y, 0),
								FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y + RoomArray[i].SY, 0)))
							{
								Why = 3;
								bCheck = false;
								break;
							}
							if (IntersectLine(FVector(Data.X, Data.Y, 0), Data.V2R,
								FVector(RoomArray[i].X, RoomArray[i].Y + RoomArray[i].SY, 0),
								FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y + RoomArray[i].SY, 0)))
							{
								Why = 3;
								bCheck = false;
								break;
							}
						}
					}
				}

				if (bCheck)
				{
					RoadArray.Add(Data);

					bCreateRoad = true;

					//UE_LOG(LogTemp, Log, TEXT("%s"), ShortRoadV1 ? *FString("true") : *FString("false"));
					//UE_LOG(LogTemp, Log, TEXT("%s"), ShortRoadV2 ? *FString("true") : *FString("false"));

					break;
				}
			}

		if (!bCreateRoad)
		{
			//방이 연결되었는지 확인하고 연결이 안됬으면 길을 새로 연결.
			RemoveEdge(&graph, edge.Vertex1, edge.Vertex2);
			UE_LOG(LogTemp, Log, TEXT("Delete Road"));

			if (!IsConnvertex(&graph, edge.Vertex1, edge.Vertex2))
			{
				int V1C = 0;
				int V2C = 0;

				for (int i = 0; i < (&graph)->NumV; i++)
				{
					if (!IsConnvertex(&graph, edge.Vertex1, i))
					{
						V1C++;
					}

					if (!IsConnvertex(&graph, i, edge.Vertex2))
					{
						V2C++;
					}
				}

				if (V1C < V2C)
				{
					float MinDis = 999999;
					int VertexIndex = 0;

					for (int i = 0; i < (&graph)->NumV; i++)
					{
						if (i != edge.Vertex1 && i != edge.Vertex2)
						{
							if (!IsConnvertex(&graph, i, edge.Vertex2))
							{
								if (!CheckEdge[i][edge.Vertex2])
								{
									float dis = FVector::Dist(FVector(RoomArray[i].X, RoomArray[i].Y, 0), FVector(RoomArray[edge.Vertex2].X, RoomArray[edge.Vertex2].Y, 0));

									if (MinDis > dis)
									{
										MinDis = dis;
										VertexIndex = i;
									}
								}
							}
						}
					}

					if (MinDis != 999999)
						AddEdge(&graph, VertexIndex, edge.Vertex2, (int)MinDis);
				}
				else
				{
					float MinDis = 999999;
					int VertexIndex = 0;

					for (int i = 0; i < (&graph)->NumV; i++)
					{
						if (i != edge.Vertex1 && i != edge.Vertex2)
						{
							if (!IsConnvertex(&graph, edge.Vertex1, i))
							{
								if (!CheckEdge[edge.Vertex1][i])
								{
									float dis = FVector::Dist(FVector(RoomArray[i].X, RoomArray[i].Y, 0), FVector(RoomArray[edge.Vertex1].X, RoomArray[edge.Vertex1].Y, 0));

									if (MinDis > dis)
									{
										MinDis = dis;
										VertexIndex = i;
									}
								}
							}
						}
					}

					if (MinDis != 999999)
						AddEdge(&graph, edge.Vertex1, VertexIndex, (int)MinDis);
				}

				UE_LOG(LogTemp, Log, TEXT("Recover Road"));
			}
		}
		else
		{
			recvEdge[eidx++] = edge;
		}
	}

	for (int i = 0; i < eidx; i++)
		PEnqueue(&(graph.Queue), recvEdge[i]);

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
	double den = (_EP2.Y - _SP2.Y)*(_EP1.X - _SP1.X) - (_EP2.X - _SP2.X)*(_EP1.Y - _SP1.Y);
	if (den == 0) return false;

	double p1 = (_EP2.X - _SP2.X)*(_SP1.Y - _SP2.Y) - (_EP2.Y - _SP2.Y)*(_SP1.X - _SP2.X);
	double p2 = (_EP1.X - _SP1.X)*(_SP1.Y - _SP2.Y) - (_EP1.Y - _SP1.Y)*(_SP1.X - _SP2.X);

	double op1 = p1 / den;
	double op2 = p2 / den;

	if (op1 < 0.0 || op1 > 1.0 || op2 < 0.0 || op2 > 1.0)
		return false;
	if (p1 == 0 && p2 == 0)
		return false;

	return true;
}
