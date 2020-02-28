// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGeneratorSys.h"
#include "Level/InGame.h"
#include "Template/RoomTemplateActor.h"
#include "Template/RoadTemplateActor.h"

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
	int iStage = 0;

	for (int i = 0; i < _Roomsize; i++)
	{
		int RandomIndex = (rand() % GameInstance->RoomTemplateData[0].Num());

		FRoomData Data;

		Data.SX = (GameInstance->RoomTemplateData[iStage][RandomIndex]->SX - 1) * 100;
		Data.SY = (GameInstance->RoomTemplateData[iStage][RandomIndex]->SY - 1) * 100;
		Data.X = 0;
		Data.Y = 0;
		
		TemplateActorArray.Add(
			InGameLevel->GetWorld()->SpawnActor<ARoomTemplateActor>(
				ARoomTemplateActor::StaticClass(),
				FTransform(FRotator(0, 0, 0),
					FVector(Data.X, Data.Y, 0),
					FVector(1, 1, 1))));

		TemplateActorArray[i]->SetAsset(GameInstance->RoomTemplateData[iStage][RandomIndex]);

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

		TArray<FRoadData> RoadLo;

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
							Data.V1R = FVector(x, RoomArray[edge.Vertex1].Y - 100, 0);
							Data.V1RF = FVector(x, RoomArray[edge.Vertex1].Y, 0);
						}
						else //길을 왼쪽으로
						{
							Data.V1R = FVector(x, RoomArray[edge.Vertex1].Y + RoomArray[edge.Vertex1].SY + 100, 0);
							Data.V1RF = FVector(x, RoomArray[edge.Vertex1].Y + RoomArray[edge.Vertex1].SY, 0);
						}
					}
					else if (RoomArray[Data.V1].Y < Data.Y &&
						RoomArray[Data.V1].Y + RoomArray[Data.V1].SY > Data.Y)
					{
						if (RoomArray[Data.V1].X > Data.X) //길을 위로
						{
							Data.V1R = FVector(RoomArray[edge.Vertex1].X - 100, y, 0);
							Data.V1RF = FVector(RoomArray[edge.Vertex1].X, y, 0);
						}
						else //길을 아래로
						{
							Data.V1R = FVector(RoomArray[edge.Vertex1].X + RoomArray[edge.Vertex1].SX + 100, y, 0);
							Data.V1RF = FVector(RoomArray[edge.Vertex1].X + RoomArray[edge.Vertex1].SX, y, 0);
						}
					}

					if (RoomArray[Data.V2].X < Data.X &&
						RoomArray[Data.V2].X + RoomArray[Data.V2].SX > Data.X)
					{
						if (RoomArray[Data.V2].Y > Data.Y) //길을 오른쪽으로
						{
							Data.V2R = FVector(x, RoomArray[edge.Vertex2].Y - 100, 0);
							Data.V2RF = FVector(x, RoomArray[edge.Vertex2].Y, 0);
						}
						else //길을 왼쪽을 
						{
							Data.V2R = FVector(x, RoomArray[edge.Vertex2].Y + RoomArray[edge.Vertex2].SY + 100, 0);
							Data.V2RF = FVector(x, RoomArray[edge.Vertex2].Y + RoomArray[edge.Vertex2].SY, 0);
						}
					}
					else if (RoomArray[Data.V2].Y < Data.Y &&
						RoomArray[Data.V2].Y + RoomArray[Data.V2].SY > Data.Y)
					{
						if (RoomArray[Data.V2].X > Data.X) //길을 위로
						{
							Data.V2R = FVector(RoomArray[edge.Vertex2].X - 100, y, 0);
							Data.V2RF = FVector(RoomArray[edge.Vertex2].X, y, 0);
						}
						else //길을 아래로
						{
							Data.V2R = FVector(RoomArray[edge.Vertex2].X + RoomArray[edge.Vertex2].SX + 100, y, 0);
							Data.V2RF = FVector(RoomArray[edge.Vertex2].X + RoomArray[edge.Vertex2].SX, y, 0);
						}
					}

					RoadLo.Add(Data);
				}
				y += 100;
			}
			x += 100;
		}

		RoadLo.Sort([](const FRoadData &A, const FRoadData &B)
		{
			float ADis = FVector::Dist(FVector(A.X, A.Y, 0), A.V1R) + FVector::Dist(FVector(A.X, A.Y, 0), A.V2R);
			float BDis = FVector::Dist(FVector(B.X, B.Y, 0), B.V1R) + FVector::Dist(FVector(B.X, B.Y, 0), B.V2R);

			return ADis < BDis;
		});

		bool bCreateRoad = false;
		int Why = 0;

		if (RoadLo.Num() > 0)
			while (wi < RoadLo.Num())
			{
				/*
				랜덤으로 뽑아서 길목 조건에 충족하는지 확인
				v1.x & v2.y || v1.y & v2.x || v1.x & v2.x || v1.y & v2.y
				*/
				Why = 0;

				bool bCheck = true;
				//int RoadIndex = rand() % RoadLo.Num();

				int x = RoadLo[wi].X;
				int y = RoadLo[wi].Y;

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

				bool ShortRoadV1 = false;
				bool ShortRoadV2 = false;

				if (RoomArray[RoadLo[wi].V1].X < RoadLo[wi].X &&
					RoomArray[RoadLo[wi].V1].X + RoomArray[RoadLo[wi].V1].SX > RoadLo[wi].X)
				{
					if (int(RoadLo[wi].V1R.Y) == RoadLo[wi].Y)
						ShortRoadV1 = true;
				}
				else if (RoomArray[RoadLo[wi].V1].Y < RoadLo[wi].Y &&
					RoomArray[RoadLo[wi].V1].Y + RoomArray[RoadLo[wi].V1].SY > RoadLo[wi].Y)
				{
					if (int(RoadLo[wi].V1R.X) == RoadLo[wi].X)
						ShortRoadV1 = true;
				}

				if (RoomArray[RoadLo[wi].V2].X < RoadLo[wi].X &&
					RoomArray[RoadLo[wi].V2].X + RoomArray[RoadLo[wi].V2].SX > RoadLo[wi].X)
				{
					if (int(RoadLo[wi].V2R.Y) == RoadLo[wi].Y)
						ShortRoadV2 = true;
				}
				else if (RoomArray[RoadLo[wi].V2].Y < RoadLo[wi].Y &&
					RoomArray[RoadLo[wi].V2].Y + RoomArray[RoadLo[wi].V2].SY > RoadLo[wi].Y)
				{
					if (int(RoadLo[wi].V2R.X) == RoadLo[wi].X)
						ShortRoadV2 = true;
				}

				/*충돌하는 길이 있는지 확인*/
				if (bCheck)
				{
					for (int i = 0; i < RoadArray.Num(); i++)
					{
						if (!ShortRoadV1)
						{
							if (IntersectLine(FVector(RoadLo[wi].X, RoadLo[wi].Y, 0), RoadLo[wi].V1R,
								FVector(RoadArray[i].X, RoadArray[i].Y, 0), RoadArray[i].V1R))
							{
								Why = 2;
								bCheck = false;
								break;
							}
							if (IntersectLine(FVector(RoadLo[wi].X, RoadLo[wi].Y, 0), RoadLo[wi].V1R,
								FVector(RoadArray[i].X, RoadArray[i].Y, 0), RoadArray[i].V2R))
							{
								Why = 2;
								bCheck = false;
								break;
							}

							if (abs(RoadLo[wi].X - RoadArray[i].X) <= 100 || abs(RoadLo[wi].Y - RoadArray[i].Y) <= 100)
							{
								bCheck = false;
								break;
							}
						} 

						if (!ShortRoadV2)
						{
							if (IntersectLine(FVector(RoadLo[wi].X, RoadLo[wi].Y, 0), RoadLo[wi].V2R,
								FVector(RoadArray[i].X, RoadArray[i].Y, 0), RoadArray[i].V1R))
							{
								Why = 2;
								bCheck = false;
								break;
							}
							if (IntersectLine(FVector(RoadLo[wi].X, RoadLo[wi].Y, 0), RoadLo[wi].V2R,
								FVector(RoadArray[i].X, RoadArray[i].Y, 0), RoadArray[i].V2R))
							{
								Why = 2;
								bCheck = false;
								break;
							}

							if (abs(RoadLo[wi].X - RoadArray[i].X) <= 100 || abs(RoadLo[wi].Y - RoadArray[i].Y) <= 100)
							{
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
							int MinX = RoadLo[wi].X;
							int MinY = RoadLo[wi].Y;
							int XABS = abs(MinX - RoadArray[i].V1R.X);
							int YABS = abs(MinY - RoadArray[i].V1R.Y);

							if (RoadLo[wi].X > RoadArray[i].V1R.X)
								MinX = RoadArray[i].V1R.X;
							if (RoadLo[wi].Y > RoadArray[i].V1R.Y)
								MinY = RoadArray[i].V1R.Y;

							for (int j = 0; j < XABS; j++)
							{
								if (MinX + j == RoadLo[wi].X && RoadArray[i].V1R.Y == RoadLo[wi].Y)
								{
									bCheck = false;
									break;
								}
							}

							for (int j = 0; j < YABS; j++)
							{
								if (MinY + j == RoadLo[wi].Y && RoadArray[i].V1R.X == RoadLo[wi].X)
								{
									bCheck = false;
									break;
								}
							}
						}

						if (!ShortRoadV2)
						{
							int MinX = RoadLo[wi].X;
							int MinY = RoadLo[wi].Y;
							int XABS = abs(MinX - RoadArray[i].V2R.X);
							int YABS = abs(MinY - RoadArray[i].V2R.Y);

							if (RoadLo[wi].X > RoadArray[i].V2R.X)
								MinX = RoadArray[i].V2R.X;
							if (RoadLo[wi].Y > RoadArray[i].V2R.Y)
								MinY = RoadArray[i].V2R.Y;

							for (int j = 0; j < XABS; j++)
							{
								if (MinX + j == RoadLo[wi].X && RoadArray[i].V2R.Y == RoadLo[wi].Y)
								{
									bCheck = false;
									break;
								}
							}

							for (int j = 0; j < YABS; j++)
							{
								if (MinY + j == RoadLo[wi].Y && RoadArray[i].V2R.X == RoadLo[wi].X)
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
							if (IntersectLine(FVector(RoadLo[wi].X, RoadLo[wi].Y, 0), RoadLo[wi].V1R,
								FVector(RoomArray[i].X, RoomArray[i].Y, 0),
								FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y, 0)))
							{
								Why = 3;
								bCheck = false;
								break;
							}
							if (IntersectLine(FVector(RoadLo[wi].X, RoadLo[wi].Y, 0), RoadLo[wi].V1R,
								FVector(RoomArray[i].X, RoomArray[i].Y, 0),
								FVector(RoomArray[i].X, RoomArray[i].Y + RoomArray[i].SY, 0)))
							{
								Why = 3;
								bCheck = false;
								break;
							}
							if (IntersectLine(FVector(RoadLo[wi].X, RoadLo[wi].Y, 0), RoadLo[wi].V1R,
								FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y, 0),
								FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y + RoomArray[i].SY, 0)))
							{
								Why = 3;
								bCheck = false;
								break;
							}
							if (IntersectLine(FVector(RoadLo[wi].X, RoadLo[wi].Y, 0), RoadLo[wi].V1R,
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
							if (IntersectLine(FVector(RoadLo[wi].X, RoadLo[wi].Y, 0), RoadLo[wi].V2R,
								FVector(RoomArray[i].X, RoomArray[i].Y, 0),
								FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y, 0)))
							{
								Why = 3;
								bCheck = false;
								break;
							}
							if (IntersectLine(FVector(RoadLo[wi].X, RoadLo[wi].Y, 0), RoadLo[wi].V2R,
								FVector(RoomArray[i].X, RoomArray[i].Y, 0),
								FVector(RoomArray[i].X, RoomArray[i].Y + RoomArray[i].SY, 0)))
							{
								Why = 3;
								bCheck = false;
								break;
							}
							if (IntersectLine(FVector(RoadLo[wi].X, RoadLo[wi].Y, 0), RoadLo[wi].V2R,
								FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y, 0),
								FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y + RoomArray[i].SY, 0)))
							{
								Why = 3;
								bCheck = false;
								break;
							}
							if (IntersectLine(FVector(RoadLo[wi].X, RoadLo[wi].Y, 0), RoadLo[wi].V2R,
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
					RoadArray.Add(RoadLo[wi]);

					bCreateRoad = true;

					break;
				}

				wi++;
			}

		if (!bCreateRoad)
		{
			bool bFind = false;
			switch (Why)
			{
			case 1:
				UE_LOG(LogTemp, Log, TEXT("Road"));
				break;
				
			case 2:
				UE_LOG(LogTemp, Log, TEXT("Collision Road"));
				break;

			case 3:
				UE_LOG(LogTemp, Log, TEXT("Collision Room"));
				break;
			}
			//방이 연결되었는지 확인하고 연결이 안됬으면 길을 새로 연결.
			RemoveEdge(&graph, edge.Vertex1, edge.Vertex2);
			UE_LOG(LogTemp, Log, TEXT("Delete Road %d %d"), edge.Vertex1, edge.Vertex2);

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
					{
						AddEdge(&graph, VertexIndex, edge.Vertex2, (int)MinDis);
						UE_LOG(LogTemp, Log, TEXT("Recover Road %d %d"), edge.Vertex1, VertexIndex);
						UE_LOG(LogTemp, Log, TEXT(" "));
						bFind = true;
					}
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
					{
						AddEdge(&graph, edge.Vertex1, VertexIndex, (int)MinDis);
						UE_LOG(LogTemp, Log, TEXT("Recover Road %d %d"), edge.Vertex1, VertexIndex);
						UE_LOG(LogTemp, Log, TEXT(" "));
						bFind = true;
					}
				}
			}

			if (!bFind)
			{
				//다른 연결 지점을 찾는다.
				float MinDis = 999999;
				int VertexIndex1 = 0;
				int VertexIndex2 = 0;

				for (int i = 0; i < (&graph)->NumV; i++)
				{
					for (int j = 0; j < (&graph)->NumV; j++)
					{
						if (i != j && !CheckEdge[i][j] && !IsConnvertex(&graph, i, j))
						{
							float dis = FVector::Dist(FVector(RoomArray[i].X, RoomArray[i].Y, 0), FVector(RoomArray[edge.Vertex1].X, RoomArray[edge.Vertex1].Y, 0));

							if (MinDis > dis)
							{
								MinDis = dis;
								VertexIndex1 = i;
								VertexIndex2 = j;
							}
						}
					}
				}

				if (MinDis != 999999)
				{
					AddEdge(&graph, VertexIndex1, VertexIndex2, (int)MinDis);
					UE_LOG(LogTemp, Log, TEXT("ReFind Road %d %d"), VertexIndex1, VertexIndex2);
					UE_LOG(LogTemp, Log, TEXT(" "));
				}
			}
		}
		else
		{
			recvEdge[eidx++] = edge;
		}
	}

	for (int i = 0; i < eidx; i++)
		PEnqueue(&(graph.Queue), recvEdge[i]);

	for (int i = 0; i < RoadArray.Num(); i++)
	{
		RoadTemplateActorArray.Add(
			InGameLevel->GetWorld()->SpawnActor<ARoadTemplateActor>(
				ARoadTemplateActor::StaticClass(),
				FTransform(FRotator(0, 0, 0),
					FVector(RoadArray[i].X, RoadArray[i].Y, 0),
					FVector(1, 1, 1))));

		int RandomIndex = (rand() % GameInstance->RoadTemplateData[iStage].Num());

		RoadTemplateActorArray[i]->SetRoadMeshData(GameInstance->RoadTemplateData[iStage][RandomIndex], RoadArray[i]);

		TemplateActorArray[RoadArray[i].V1]->CreateRoad(RoadArray[i].V1R, RoadArray[i].V1RF);
		TemplateActorArray[RoadArray[i].V2]->CreateRoad(RoadArray[i].V2R, RoadArray[i].V2RF);
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
