// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGeneratorSys.h"
#include "Level/InGame.h"
#include "Template/RoomTemplateActor.h"
#include "Template/RoadTemplateActor.h"
#include "MyGameInstance.h"
#include "InteractionActor/MonsterSpawn.h"

MapGeneratorSys::MapGeneratorSys(class AInGame* _InGameLevel)
{
	InGameLevel = _InGameLevel;
	GameInstance = InGameLevel->GetMyGameInstance();
}

MapGeneratorSys::~MapGeneratorSys()
{
}

void MapGeneratorSys::MapGen(int _Stage)
{
	iStage = _Stage;

	//방 스폰
	int RoomSize = 0;
	int RoomCount = 0;

	while (GameInstance->StageSizeData[_Stage].StageSize > RoomSize) //다른 방들 생성
	{
		int RandomIndex = (rand() % GameInstance->RoomTemplateData[iStage].Num());

		if (!GameInstance->RoomTemplateData[iStage][RandomIndex].IsBossRoom)
		{
			UE_LOG(LogTemp, Log, TEXT("RoomIndex %d"), RandomIndex);

			FRoomData Data;

			Data.SX = (GameInstance->RoomTemplateData[iStage][RandomIndex].Template->SX - 1) * 100;
			Data.SY = (GameInstance->RoomTemplateData[iStage][RandomIndex].Template->SY - 1) * 100;
			Data.X = 0;
			Data.Y = 0;
			Data.RoomNumber = RandomIndex;

			TemplateActorArray.Add(
				InGameLevel->GetWorld()->SpawnActor<ARoomTemplateActor>(
					ARoomTemplateActor::StaticClass(),
					FTransform(FRotator(0, 0, 0),
						FVector(Data.X, Data.Y, 0),
						FVector(1, 1, 1))));

			TemplateActorArray[RoomCount]->SetAsset(GameInstance->RoomTemplateData[iStage][RandomIndex].Template);

			RoomArray.Add(Data);

			RoomSize += GameInstance->RoomTemplateData[iStage][RandomIndex].RoomMonsterMaxScore;
			RoomCount++;
		}
	}

	while (true) //보스방 생성
	{
		int RandomIDX = (rand() % GameInstance->RoomTemplateData[iStage].Num());

		if (GameInstance->RoomTemplateData[iStage][RandomIDX].IsBossRoom)
		{
			FRoomData Data;

			Data.SX = (GameInstance->RoomTemplateData[iStage][RandomIDX].Template->SX - 1) * 100;
			Data.SY = (GameInstance->RoomTemplateData[iStage][RandomIDX].Template->SY - 1) * 100;
			Data.X = 0;
			Data.Y = 0;
			Data.RoomNumber = RandomIDX;

			TemplateActorArray.Add(
				InGameLevel->GetWorld()->SpawnActor<ARoomTemplateActor>(
					ARoomTemplateActor::StaticClass(),
					FTransform(FRotator(0, 0, 0),
						FVector(Data.X, Data.Y, 0),
						FVector(1, 1, 1))));

			TemplateActorArray[RoomCount]->SetAsset(GameInstance->RoomTemplateData[iStage][RandomIDX].Template);

			RoomArray.Add(Data);

			RoomCount++;
			break;
		}
	}
	//출발방 생성

	FRoomData Data;

	Data.SX = (GameInstance->RoomTemplateData[iStage][0].Template->SX - 1) * 100;
	Data.SY = (GameInstance->RoomTemplateData[iStage][0].Template->SY - 1) * 100;
	Data.X = 0;
	Data.Y = 0;

	TemplateActorArray.Add(
		InGameLevel->GetWorld()->SpawnActor<ARoomTemplateActor>(
			ARoomTemplateActor::StaticClass(),
			FTransform(FRotator(0, 0, 0),
				FVector(Data.X, Data.Y, 0),
				FVector(1, 1, 1))));

	TemplateActorArray[RoomCount]->SetAsset(GameInstance->RoomTemplateData[iStage][0].Template);

	RoomArray.Add(Data);

	RoomCount++;
	//출발방 생성

	//방이 겹치지 않게 퍼트림

	int FarIdx = 0;
	float FarDis = 0;
	for (int i = 0; i < RoomArray.Num(); i++)
	{
		while (true)
		{
			bool Check = true;

			for (int x = 0; x < RoomArray.Num(); x++)
			{
				if (i != x && i != RoomArray.Num() - 2 && x != RoomArray.Num() - 2)
				{
					int MinSX = RoomArray[i].SX;
					int MinSY = RoomArray[i].SY;

					if (RoomArray[i].X > RoomArray[x].X)
						MinSX = RoomArray[x].SX;
					if (RoomArray[i].Y > RoomArray[x].Y)
						MinSY = RoomArray[x].SY;

					if (abs(RoomArray[i].X - RoomArray[x].X) <= (MinSX + 100) &&
						abs(RoomArray[i].Y - RoomArray[x].Y) <= (MinSY + 100))
					{
						RoomArray[i].X += ((rand() % 21) - 10) * 100;
						RoomArray[i].Y += ((rand() % 21) - 10) * 100;

						TemplateActorArray[i]->SetActorLocation(FVector(RoomArray[i].X, RoomArray[i].Y, 0));

						if (FarDis < FVector::Distance(FVector(0, 0, 0), FVector(RoomArray[i].X, RoomArray[i].Y, 0)))
						{
							FarDis = FVector::Distance(FVector(0, 0, 0), FVector(RoomArray[i].X, RoomArray[i].Y, 0));
							FarIdx = i;
						}

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

	TemplateActorArray[RoomArray.Num() - 2]->SetActorLocation(FVector(RoomArray[FarIdx].X, RoomArray[FarIdx].Y, 0));
	while (true)
	{
		bool Check = true;

		for (int x = 0; x < RoomArray.Num(); x++)
		{
			if (x != RoomArray.Num() - 2)
			{
				int MinSX = RoomArray[RoomArray.Num() - 2].SX;
				int MinSY = RoomArray[RoomArray.Num() - 2].SY;

				if (RoomArray[RoomArray.Num() - 2].X > RoomArray[x].X)
					MinSX = RoomArray[x].SX;
				if (RoomArray[RoomArray.Num() - 2].Y > RoomArray[x].Y)
					MinSY = RoomArray[x].SY;

				if (abs(RoomArray[RoomArray.Num() - 2].X - RoomArray[x].X) <= (MinSX + 100) &&
					abs(RoomArray[RoomArray.Num() - 2].Y - RoomArray[x].Y) <= (MinSY + 100))
				{
					int XMul = 1;
					int YMul = 1;

					if (RoomArray[FarIdx].X < 0)
						XMul = -1;
					if (RoomArray[FarIdx].Y < 0)
						YMul = -1;

					RoomArray[RoomArray.Num() - 2].X += ((rand() % 10)) * (100 * XMul);
					RoomArray[RoomArray.Num() - 2].Y += ((rand() % 10)) * (100 * YMul);

					TemplateActorArray[RoomArray.Num() - 2]->SetActorLocation(FVector(RoomArray[RoomArray.Num() - 2].X, RoomArray[RoomArray.Num() - 2].Y, 0));

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

	GraphInit(&graph, RoomCount);

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
	FPQueue* copyPQ = &graph.Queue;
	FGraphEdge recvEdge[ARR_SIZE];
	int eidx = 0;
	FGraphEdge edge;
	bool CheckEdge[256][256] = { false };

	while (!PQIsEmpty(copyPQ))
	{
		edge = PDequeue(copyPQ);

		FRoomData RData;

		CheckEdge[edge.Vertex1][edge.Vertex2] = true;

		if (RoomArray[edge.Vertex1].X > RoomArray[edge.Vertex2].X)
		{
			RData.X = RoomArray[edge.Vertex2].X;
			RData.SX = (RoomArray[edge.Vertex1].X + RoomArray[edge.Vertex1].SX) - RoomArray[edge.Vertex2].X;
		}
		else
		{
			RData.X = RoomArray[edge.Vertex1].X;
			RData.SX = (RoomArray[edge.Vertex2].X + RoomArray[edge.Vertex2].SX) - RoomArray[edge.Vertex1].X;
		}

		if (RoomArray[edge.Vertex1].Y > RoomArray[edge.Vertex2].Y)
		{
			RData.Y = RoomArray[edge.Vertex2].Y;
			RData.SY = (RoomArray[edge.Vertex1].Y + RoomArray[edge.Vertex1].SY) - RoomArray[edge.Vertex2].Y;
		}
		else
		{
			RData.Y = RoomArray[edge.Vertex1].Y;
			RData.SY = (RoomArray[edge.Vertex2].Y + RoomArray[edge.Vertex2].SY) - RoomArray[edge.Vertex1].Y;
		}

		TArray<FRoadData> RoadLo;

		int wi = 0;

		int V1X = RoomArray[edge.Vertex1].X + 100;
		int V1SX = RoomArray[edge.Vertex1].SX - 100;
		int V1Y = RoomArray[edge.Vertex1].Y + 100;
		int V1SY = RoomArray[edge.Vertex1].SY - 100;

		int V2X = RoomArray[edge.Vertex2].X + 100;
		int V2SX = RoomArray[edge.Vertex2].SX - 100;
		int V2Y = RoomArray[edge.Vertex2].Y + 100;
		int V2SY = RoomArray[edge.Vertex2].SY - 100;

		for (int x = RData.X; x < RData.X + RData.SX;)
		{
			for (int y = RData.Y; y < RData.Y + RData.SY;)
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
					Data.Data.Init(FRoadDataData(), 2);

					Data.Data[0].V1 = edge.Vertex1;
					Data.Data[0].V2 = edge.Vertex2;
					Data.Data[0].X = x;
					Data.Data[0].Y = y;

					Data.Data[1].V1 = edge.Vertex1;
					Data.Data[1].V2 = edge.Vertex2;
					Data.Data[1].X = x;
					Data.Data[1].Y = y;

					if (RoomArray[Data.Data[0].V1].X < Data.Data[0].X &&
						RoomArray[Data.Data[0].V1].X + RoomArray[Data.Data[0].V1].SX > Data.Data[0].X)
					{
						if (RoomArray[Data.Data[0].V1].Y > Data.Data[0].Y) //길을 오른쪽으로
						{
							Data.Data[0].V1R = FVector(x, RoomArray[edge.Vertex1].Y - 100, 0);
							Data.Data[0].V1RF = FVector(x, RoomArray[edge.Vertex1].Y, 0);
							Data.Data[0].V1RFF = FVector(x, RoomArray[edge.Vertex1].Y + 100, 0);

							Data.Data[1].Y -= 100;
						}
						else //길을 왼쪽으로
						{
							Data.Data[0].V1R = FVector(x, RoomArray[edge.Vertex1].Y + RoomArray[edge.Vertex1].SY + 100, 0);
							Data.Data[0].V1RF = FVector(x, RoomArray[edge.Vertex1].Y + RoomArray[edge.Vertex1].SY, 0);
							Data.Data[0].V1RFF = FVector(x, RoomArray[edge.Vertex1].Y + RoomArray[edge.Vertex1].SY - 100, 0);

							Data.Data[1].Y += 100;
						}
					}
					else if (RoomArray[Data.Data[0].V1].Y < Data.Data[0].Y &&
						RoomArray[Data.Data[0].V1].Y + RoomArray[Data.Data[0].V1].SY > Data.Data[0].Y)
					{
						if (RoomArray[Data.Data[0].V1].X > Data.Data[0].X) //길을 위로
						{
							Data.Data[0].V1R = FVector(RoomArray[edge.Vertex1].X - 100, y, 0);
							Data.Data[0].V1RF = FVector(RoomArray[edge.Vertex1].X, y, 0);
							Data.Data[0].V1RFF = FVector(RoomArray[edge.Vertex1].X + 100, y, 0);

							Data.Data[1].X -= 100;
						}
						else //길을 아래로
						{
							Data.Data[0].V1R = FVector(RoomArray[edge.Vertex1].X + RoomArray[edge.Vertex1].SX + 100, y, 0);
							Data.Data[0].V1RF = FVector(RoomArray[edge.Vertex1].X + RoomArray[edge.Vertex1].SX, y, 0);
							Data.Data[0].V1RFF = FVector(RoomArray[edge.Vertex1].X + RoomArray[edge.Vertex1].SX - 100, y, 0);

							Data.Data[1].X += 100;
						}
					}

					if (RoomArray[Data.Data[0].V2].X < Data.Data[0].X &&
						RoomArray[Data.Data[0].V2].X + RoomArray[Data.Data[0].V2].SX > Data.Data[0].X)
					{
						if (RoomArray[Data.Data[0].V2].Y > Data.Data[0].Y) //길을 오른쪽으로
						{
							Data.Data[0].V2R = FVector(x, RoomArray[edge.Vertex2].Y - 100, 0);
							Data.Data[0].V2RF = FVector(x, RoomArray[edge.Vertex2].Y, 0);
							Data.Data[0].V2RFF = FVector(x, RoomArray[edge.Vertex2].Y + 100, 0);

							Data.Data[1].Y -= 100;
						}
						else //길을 왼쪽을 
						{
							Data.Data[0].V2R = FVector(x, RoomArray[edge.Vertex2].Y + RoomArray[edge.Vertex2].SY + 100, 0);
							Data.Data[0].V2RF = FVector(x, RoomArray[edge.Vertex2].Y + RoomArray[edge.Vertex2].SY, 0);
							Data.Data[0].V2RFF = FVector(x, RoomArray[edge.Vertex2].Y + RoomArray[edge.Vertex2].SY - 100, 0);

							Data.Data[1].Y += 100;
						}
					}
					else if (RoomArray[Data.Data[0].V2].Y < Data.Data[0].Y &&
						RoomArray[Data.Data[0].V2].Y + RoomArray[Data.Data[0].V2].SY > Data.Data[0].Y)
					{
						if (RoomArray[Data.Data[0].V2].X > Data.Data[0].X) //길을 위로
						{
							Data.Data[0].V2R = FVector(RoomArray[edge.Vertex2].X - 100, y, 0);
							Data.Data[0].V2RF = FVector(RoomArray[edge.Vertex2].X, y, 0);
							Data.Data[0].V2RFF = FVector(RoomArray[edge.Vertex2].X + 100, y, 0);

							Data.Data[1].X -= 100;
						}
						else //길을 아래로
						{
							Data.Data[0].V2R = FVector(RoomArray[edge.Vertex2].X + RoomArray[edge.Vertex2].SX + 100, y, 0);
							Data.Data[0].V2RF = FVector(RoomArray[edge.Vertex2].X + RoomArray[edge.Vertex2].SX, y, 0);
							Data.Data[0].V2RFF = FVector(RoomArray[edge.Vertex2].X + RoomArray[edge.Vertex2].SX - 100, y, 0);

							Data.Data[1].X += 100;
						}
					}

					if (Data.Data[0].X == Data.Data[1].X && Data.Data[0].V1R.X == Data.Data[0].V1RF.X)
						Data.Data[1].X -= 100;
					else if (Data.Data[0].Y == Data.Data[1].Y && Data.Data[0].V1R.Y == Data.Data[0].V1RF.Y)
						Data.Data[1].Y -= 100;

					Data.Data[1].V1R = Data.Data[0].V1R;
					Data.Data[1].V1RF = Data.Data[0].V1RF;
					Data.Data[1].V1RFF = Data.Data[0].V1RFF;

					Data.Data[1].V2R = Data.Data[0].V2R;
					Data.Data[1].V2RF = Data.Data[0].V2RF;
					Data.Data[1].V2RFF = Data.Data[0].V2RFF;

					if (Data.Data[1].V1R.X == Data.Data[1].V1RF.X)
					{
						Data.Data[1].V1R.X += Data.Data[1].X - Data.Data[0].X;
						Data.Data[1].V1RF.X += Data.Data[1].X - Data.Data[0].X;
						Data.Data[1].V1RFF.X += Data.Data[1].X - Data.Data[0].X;
					}
					else if (Data.Data[1].V1R.Y == Data.Data[1].V1RF.Y)
					{
						Data.Data[1].V1R.Y += Data.Data[1].Y - Data.Data[0].Y;
						Data.Data[1].V1RF.Y += Data.Data[1].Y - Data.Data[0].Y;
						Data.Data[1].V1RFF.Y += Data.Data[1].Y - Data.Data[0].Y;
					}

					if (Data.Data[1].V2R.X == Data.Data[1].V2RF.X)
					{
						Data.Data[1].V2R.X += Data.Data[1].X - Data.Data[0].X;
						Data.Data[1].V2RF.X += Data.Data[1].X - Data.Data[0].X;
						Data.Data[1].V2RFF.X += Data.Data[1].X - Data.Data[0].X;
					}
					else if (Data.Data[1].V2R.Y == Data.Data[1].V2RF.Y)
					{
						Data.Data[1].V2R.Y += Data.Data[1].Y - Data.Data[0].Y;
						Data.Data[1].V2RF.Y += Data.Data[1].Y - Data.Data[0].Y;
						Data.Data[1].V2RFF.Y += Data.Data[1].Y - Data.Data[0].Y;
					}

					if ((
						(V1X < Data.Data[1].X && V1X + V1SX > Data.Data[1].X && V2Y < Data.Data[1].Y && V2Y + V2SY > Data.Data[1].Y) ||
						(V1Y < Data.Data[1].Y && V1Y + V1SY > Data.Data[1].Y && V2X < Data.Data[1].X && V2X + V2SX > Data.Data[1].X) ||
						(V1X < Data.Data[1].X && V1X + V1SX > Data.Data[1].X && V2X < Data.Data[1].X && V2X + V2SX > Data.Data[1].X) ||
						(V1Y < Data.Data[1].Y && V1Y + V1SY > Data.Data[1].Y && V2Y < Data.Data[1].Y && V2Y + V2SY > Data.Data[1].Y)) &&
						!(V1X <= Data.Data[1].X && V1X + V1SX >= Data.Data[1].X && V1Y <= Data.Data[1].Y && V1Y + V1SY >= Data.Data[1].Y) &&
						!(V2X <= Data.Data[1].X && V2X + V2SX >= Data.Data[1].X && V2Y <= Data.Data[1].Y && V2Y + V2SY >= Data.Data[1].Y))
					{
						RoadLo.Add(Data);
					}
				}
				y += 100;
			}
			x += 100;
		}

		RoadLo.Sort([](const FRoadData& A, const FRoadData& B)
			{
				float ADis = FVector::Dist(FVector(A.Data[0].X, A.Data[0].Y, 0), A.Data[0].V1R) + FVector::Dist(FVector(A.Data[0].X, A.Data[0].Y, 0), A.Data[0].V2R);
				float BDis = FVector::Dist(FVector(B.Data[0].X, B.Data[0].Y, 0), B.Data[0].V1R) + FVector::Dist(FVector(B.Data[0].X, B.Data[0].Y, 0), B.Data[0].V2R);

				return ADis < BDis;
			});

		bool bCreateRoad = false;
		int Why = 0;

		if (!((edge.Vertex1 == RoomArray.Num() - 1 || edge.Vertex1 == RoomArray.Num() - 2) &&
			(edge.Vertex2 == RoomArray.Num() - 1 || edge.Vertex2 == RoomArray.Num() - 2)))
			if (RoadLo.Num() > 0)
			{
				while (wi < RoadLo.Num())
				{
					Why = 0;

					bool bCheck = true;
					//int RoadIndex = rand() % RoadLo.Num();
					for (int fiCount = 0; fiCount < 2; fiCount++)
					{
						int x = RoadLo[wi].Data[fiCount].X;
						int y = RoadLo[wi].Data[fiCount].Y;

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
							if (RoadArray[i].Data[fiCount].X == x && RoadArray[i].Data[fiCount].Y && y)
							{
								bCheck = false;
								break;
							}
						}

						bool ShortRoadV1 = false;
						bool ShortRoadV2 = false;

						if (RoomArray[RoadLo[wi].Data[fiCount].V1].X < RoadLo[wi].Data[fiCount].X &&
							RoomArray[RoadLo[wi].Data[fiCount].V1].X + RoomArray[RoadLo[wi].Data[fiCount].V1].SX > RoadLo[wi].Data[fiCount].X)
						{
							if (int(RoadLo[wi].Data[fiCount].V1R.Y) == RoadLo[wi].Data[fiCount].Y)
								ShortRoadV1 = true;
						}
						else if (RoomArray[RoadLo[wi].Data[fiCount].V1].Y < RoadLo[wi].Data[fiCount].Y &&
							RoomArray[RoadLo[wi].Data[fiCount].V1].Y + RoomArray[RoadLo[wi].Data[fiCount].V1].SY > RoadLo[wi].Data[fiCount].Y)
						{
							if (int(RoadLo[wi].Data[fiCount].V1R.X) == RoadLo[wi].Data[fiCount].X)
								ShortRoadV1 = true;
						}

						if (RoomArray[RoadLo[wi].Data[fiCount].V2].X < RoadLo[wi].Data[fiCount].X &&
							RoomArray[RoadLo[wi].Data[fiCount].V2].X + RoomArray[RoadLo[wi].Data[fiCount].V2].SX > RoadLo[wi].Data[fiCount].X)
						{
							if (int(RoadLo[wi].Data[fiCount].V2R.Y) == RoadLo[wi].Data[fiCount].Y)
								ShortRoadV2 = true;
						}
						else if (RoomArray[RoadLo[wi].Data[fiCount].V2].Y < RoadLo[wi].Data[fiCount].Y &&
							RoomArray[RoadLo[wi].Data[fiCount].V2].Y + RoomArray[RoadLo[wi].Data[fiCount].V2].SY > RoadLo[wi].Data[fiCount].Y)
						{
							if (int(RoadLo[wi].Data[fiCount].V2R.X) == RoadLo[wi].Data[fiCount].X)
								ShortRoadV2 = true;
						}

						/*충돌하는 길이 있는지 확인*/
						if (bCheck)
						{
							for (int i = 0; i < RoadArray.Num(); i++)
							{
								if (!ShortRoadV1)
								{
									if (IntersectLine(FVector(RoadLo[wi].Data[fiCount].X, RoadLo[wi].Data[fiCount].Y, 0), RoadLo[wi].Data[fiCount].V1R,
										FVector(RoadArray[i].Data[fiCount].X, RoadArray[i].Data[fiCount].Y, 0), RoadArray[i].Data[fiCount].V1R))
									{
										Why = 2;
										bCheck = false;
										break;
									}
									if (IntersectLine(FVector(RoadLo[wi].Data[fiCount].X, RoadLo[wi].Data[fiCount].Y, 0), RoadLo[wi].Data[fiCount].V1R,
										FVector(RoadArray[i].Data[fiCount].X, RoadArray[i].Data[fiCount].Y, 0), RoadArray[i].Data[fiCount].V2R))
									{
										Why = 2;
										bCheck = false;
										break;
									}
								}

								if (!ShortRoadV2)
								{
									if (IntersectLine(FVector(RoadLo[wi].Data[fiCount].X, RoadLo[wi].Data[fiCount].Y, 0), RoadLo[wi].Data[fiCount].V2R,
										FVector(RoadArray[i].Data[fiCount].X, RoadArray[i].Data[fiCount].Y, 0), RoadArray[i].Data[fiCount].V1R))
									{
										Why = 2;
										bCheck = false;
										break;
									}
									if (IntersectLine(FVector(RoadLo[wi].Data[fiCount].X, RoadLo[wi].Data[fiCount].Y, 0), RoadLo[wi].Data[fiCount].V2R,
										FVector(RoadArray[i].Data[fiCount].X, RoadArray[i].Data[fiCount].Y, 0), RoadArray[i].Data[fiCount].V2R))
									{
										Why = 2;
										bCheck = false;
										break;
									}
								}

								if (abs(RoadLo[wi].Data[fiCount].V2R.X - RoadArray[i].Data[fiCount].V2R.X) <= 300 &&
									abs(RoadLo[wi].Data[fiCount].V2R.Y - RoadArray[i].Data[fiCount].V2R.Y) <= 300)
								{
									bCheck = false;
									break;
								}

								if (abs(RoadLo[wi].Data[fiCount].V1R.X - RoadArray[i].Data[fiCount].V1R.X) <= 300 &&
									abs(RoadLo[wi].Data[fiCount].V1R.Y - RoadArray[i].Data[fiCount].V1R.Y) <= 300)
								{
									bCheck = false;
									break;
								}

								if (abs(RoadLo[wi].Data[fiCount].V1R.X - RoadArray[i].Data[fiCount].V2R.X) <= 300 &&
									abs(RoadLo[wi].Data[fiCount].V1R.Y - RoadArray[i].Data[fiCount].V2R.Y) <= 300)
								{
									bCheck = false;
									break;
								}

								if (abs(RoadLo[wi].Data[fiCount].V2R.X - RoadArray[i].Data[fiCount].V1R.X) <= 300 &&
									abs(RoadLo[wi].Data[fiCount].V2R.Y - RoadArray[i].Data[fiCount].V1R.Y) <= 300)
								{
									bCheck = false;
									break;
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
									int MinX = RoadLo[wi].Data[fiCount].X;
									int MinY = RoadLo[wi].Data[fiCount].Y;
									int XABS = abs(MinX - RoadArray[i].Data[fiCount].V1R.X);
									int YABS = abs(MinY - RoadArray[i].Data[fiCount].V1R.Y);

									if (RoadLo[wi].Data[fiCount].X > RoadArray[i].Data[fiCount].V1R.X)
										MinX = RoadArray[i].Data[fiCount].V1R.X;
									if (RoadLo[wi].Data[fiCount].Y > RoadArray[i].Data[fiCount].V1R.Y)
										MinY = RoadArray[i].Data[fiCount].V1R.Y;

									for (int j = 0; j < XABS; j++)
									{
										if (MinX + j == RoadLo[wi].Data[fiCount].X &&
											RoadArray[i].Data[fiCount].V1R.Y == RoadLo[wi].Data[fiCount].Y)
										{
											bCheck = false;
											break;
										}
									}

									for (int j = 0; j < YABS; j++)
									{
										if (MinY + j == RoadLo[wi].Data[fiCount].Y &&
											RoadArray[i].Data[fiCount].V1R.X == RoadLo[wi].Data[fiCount].X)
										{
											bCheck = false;
											break;
										}
									}
								}

								if (!ShortRoadV2)
								{
									int MinX = RoadLo[wi].Data[fiCount].X;
									int MinY = RoadLo[wi].Data[fiCount].Y;
									int XABS = abs(MinX - RoadArray[i].Data[fiCount].V2R.X);
									int YABS = abs(MinY - RoadArray[i].Data[fiCount].V2R.Y);

									if (RoadLo[wi].Data[fiCount].X > RoadArray[i].Data[fiCount].V2R.X)
										MinX = RoadArray[i].Data[fiCount].V2R.X;
									if (RoadLo[wi].Data[fiCount].Y > RoadArray[i].Data[fiCount].V2R.Y)
										MinY = RoadArray[i].Data[fiCount].V2R.Y;

									for (int j = 0; j < XABS; j++)
									{
										if (MinX + j == RoadLo[wi].Data[fiCount].X &&
											RoadArray[i].Data[fiCount].V2R.Y == RoadLo[wi].Data[fiCount].Y)
										{
											bCheck = false;
											break;
										}
									}

									for (int j = 0; j < YABS; j++)
									{
										if (MinY + j == RoadLo[wi].Data[fiCount].Y &&
											RoadArray[i].Data[fiCount].V2R.X == RoadLo[wi].Data[fiCount].X)
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
									if (IntersectLine(FVector(RoadLo[wi].Data[fiCount].X, RoadLo[wi].Data[fiCount].Y, 0), RoadLo[wi].Data[fiCount].V1R,
										FVector(RoomArray[i].X, RoomArray[i].Y, 0),
										FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y, 0)))
									{
										Why = 3;
										bCheck = false;
										break;
									}
									if (IntersectLine(FVector(RoadLo[wi].Data[fiCount].X, RoadLo[wi].Data[fiCount].Y, 0), RoadLo[wi].Data[fiCount].V1R,
										FVector(RoomArray[i].X, RoomArray[i].Y, 0),
										FVector(RoomArray[i].X, RoomArray[i].Y + RoomArray[i].SY, 0)))
									{
										Why = 3;
										bCheck = false;
										break;
									}
									if (IntersectLine(FVector(RoadLo[wi].Data[fiCount].X, RoadLo[wi].Data[fiCount].Y, 0), RoadLo[wi].Data[fiCount].V1R,
										FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y, 0),
										FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y + RoomArray[i].SY, 0)))
									{
										Why = 3;
										bCheck = false;
										break;
									}
									if (IntersectLine(FVector(RoadLo[wi].Data[fiCount].X, RoadLo[wi].Data[fiCount].Y, 0), RoadLo[wi].Data[fiCount].V1R,
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
									if (IntersectLine(FVector(RoadLo[wi].Data[fiCount].X, RoadLo[wi].Data[fiCount].Y, 0), RoadLo[wi].Data[fiCount].V2R,
										FVector(RoomArray[i].X, RoomArray[i].Y, 0),
										FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y, 0)))
									{
										Why = 3;
										bCheck = false;
										break;
									}
									if (IntersectLine(FVector(RoadLo[wi].Data[fiCount].X, RoadLo[wi].Data[fiCount].Y, 0), RoadLo[wi].Data[fiCount].V2R,
										FVector(RoomArray[i].X, RoomArray[i].Y, 0),
										FVector(RoomArray[i].X, RoomArray[i].Y + RoomArray[i].SY, 0)))
									{
										Why = 3;
										bCheck = false;
										break;
									}
									if (IntersectLine(FVector(RoadLo[wi].Data[fiCount].X, RoadLo[wi].Data[fiCount].Y, 0), RoadLo[wi].Data[fiCount].V2R,
										FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y, 0),
										FVector(RoomArray[i].X + RoomArray[i].SX, RoomArray[i].Y + RoomArray[i].SY, 0)))
									{
										Why = 3;
										bCheck = false;
										break;
									}
									if (IntersectLine(FVector(RoadLo[wi].Data[fiCount].X, RoadLo[wi].Data[fiCount].Y, 0), RoadLo[wi].Data[fiCount].V2R,
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
					}

					if (bCheck)
					{
						RoadArray.Add(RoadLo[wi]);

						bCreateRoad = true;

						break;
					}

					wi++;
				}
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("No RoadLo"));
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
			//UE_LOG(LogTemp, Log, TEXT("Cant Create Road %d %d"), edge.Vertex2, edge.Vertex1);
			//방이 연결되었는지 확인하고 연결이 안됬으면 길을 새로 연결.
			RemoveEdge(&graph, edge.Vertex1, edge.Vertex2);

			if (!IsConnvertex(&graph, edge.Vertex1, edge.Vertex2))
			{
				int V1C = 0;
				int V2C = 0;

				TArray<int> V1CEArr;
				TArray<int> V2CEArr;

				V1CEArr.Add(edge.Vertex1);
				V2CEArr.Add(edge.Vertex2);

				for (int i = 0; i < (&graph)->NumV; i++)
				{
					if (!IsConnvertex(&graph, edge.Vertex1, i))
						V1C++;
					else
						V1CEArr.Add(i);

					if (!IsConnvertex(&graph, i, edge.Vertex2))
						V2C++;
					else
						V2CEArr.Add(i);
				}

				if (V1C > V2C)
				{
					float MinDis = -1;
					int VertexIndex1 = -1;
					int VertexIndex2 = -1;

					for (int EArr = 0; EArr < V2CEArr.Num(); EArr++)
						for (int i = 0; i < (&graph)->NumV; i++)
							if (i != V2CEArr[EArr])
								if (!IsConnvertex(&graph, i, V2CEArr[EArr]) && !CheckEdge[i][V2CEArr[EArr]])
								{
									float dis = FVector::Dist(FVector(RoomArray[i].X, RoomArray[i].Y, 0), FVector(RoomArray[V2CEArr[EArr]].X, RoomArray[V2CEArr[EArr]].Y, 0));

									if (MinDis > dis || MinDis == -1)
									{
										MinDis = dis;
										VertexIndex1 = i;
										VertexIndex2 = V2CEArr[EArr];
									}
								}

					if (MinDis != -1 && VertexIndex1 != -1 && VertexIndex2 != -1)
					{
						AddEdge(&graph, VertexIndex1, VertexIndex2, (int)MinDis);
						bFind = true;
					}
				}
				else
				{
					float MinDis = -1;
					int VertexIndex1 = -1;
					int VertexIndex2 = -1;

					for (int EArr = 0; EArr < V1CEArr.Num(); EArr++)
						for (int i = 0; i < (&graph)->NumV; i++)
							if (i != V1CEArr[EArr])
								if (!IsConnvertex(&graph, V1CEArr[EArr], i) && !CheckEdge[V1CEArr[EArr]][i])
								{
									float dis = FVector::Dist(FVector(RoomArray[i].X, RoomArray[i].Y, 0), FVector(RoomArray[V1CEArr[EArr]].X, RoomArray[V1CEArr[EArr]].Y, 0));

									if (MinDis > dis || MinDis == -1)
									{
										MinDis = dis;
										VertexIndex2 = i;
										VertexIndex1 = V1CEArr[EArr];
									}
								}

					if (MinDis != -1 && VertexIndex1 != -1 && VertexIndex2 != -1)
					{
						AddEdge(&graph, VertexIndex1, VertexIndex2, (int)MinDis);
						bFind = true;
					}
				}

				if (!bFind)
				{
					if (!CheckEdge[edge.Vertex2][edge.Vertex1])
					{
						AddEdge(&graph, edge.Vertex2, edge.Vertex1, 0);
						bFind = true;
					}
					else
						UE_LOG(LogTemp, Log, TEXT("False Road %d %d %d %d"), edge.Vertex1, edge.Vertex2, RoadArray.Num(), eidx);
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

	/*입구 설정 및 길 설치하는 코드*/
	for (int fiCount = 0; fiCount < 2; fiCount++)
	{
		for (int i = 0; i < RoadArray.Num(); i++)
		{
			RoomActiveActorArray.Add(TArray<ARoomActiveActor*>());

			RoadTemplateActorArray.Add(
				InGameLevel->GetWorld()->SpawnActor<ARoadTemplateActor>(
					ARoadTemplateActor::StaticClass(),
					FTransform(FRotator(0, 0, 0),
						FVector(RoadArray[i].Data[fiCount].X, RoadArray[i].Data[fiCount].Y, 0),
						FVector(1, 1, 1))));

			int RandomIndex = (rand() % GameInstance->RoadTemplateData[iStage].Num());

			RoadTemplateActorArray[i]->SetRoadMeshData(GameInstance->RoadTemplateData[iStage][RandomIndex], RoadArray[i]);

			TemplateActorArray[RoadArray[i].Data[fiCount].V1]->CreateRoad(RoadArray[i].Data[fiCount].V1R, RoadArray[i].Data[fiCount].V1RF);
			TemplateActorArray[RoadArray[i].Data[fiCount].V2]->CreateRoad(RoadArray[i].Data[fiCount].V2R, RoadArray[i].Data[fiCount].V2RF);

			FVector V1RF = RoadArray[i].Data[fiCount].V1RF;
			FVector V2RF = RoadArray[i].Data[fiCount].V2RF;

			if (fiCount == 0)
			{
				if (RoadArray[i].Data[fiCount].X == V1RF.X)
				{
					float Rot = 0;
					if (RoadArray[i].Data[1].X > V1RF.X)
						Rot = 180;
					else
						Rot = 0;

					DoorArray.Add(InGameLevel->GetWorld()->SpawnActor<ARoomDoor>(
						ARoomDoor::StaticClass(),
						FTransform(FRotator(0, Rot, 0),
							V1RF,
							FVector(1, 1, 1))));

					if (CheckBossRoom(i, fiCount)) //보스방 세팅
						DoorArray[DoorArray.Num() - 1]->BossDoor();

					if (RoadArray[i].Data[fiCount].V1 != RoomArray.Num() - 1)
					{
						RoomActiveActorArray[i].Add(
							InGameLevel->GetWorld()->SpawnActor<ARoomActiveActor>(
								ARoomActiveActor::StaticClass(),
								FTransform(FRotator(0, 0, 0),
									RoadArray[i].Data[fiCount].V1RFF,
									FVector(1, 1, 1))));

						RoomActiveActorArray[i][RoomActiveActorArray[i].Num() - 1]->SetRoomNumber(RoadArray[i].Data[fiCount].V1);
					}
				}
				else if (RoadArray[i].Data[fiCount].Y == V1RF.Y)
				{
					float Rot = 0;
					if (RoadArray[i].Data[1].Y > V1RF.Y)
						Rot = -90;
					else
						Rot = 90;

					DoorArray.Add(InGameLevel->GetWorld()->SpawnActor<ARoomDoor>(
						ARoomDoor::StaticClass(),
						FTransform(FRotator(0, Rot, 0),
							V1RF,
							FVector(1, 1, 1))));

					if (CheckBossRoom(i, fiCount)) //보스방 세팅
						DoorArray[DoorArray.Num() - 1]->BossDoor();

					if (RoadArray[i].Data[fiCount].V1 != RoomArray.Num() - 1)
					{
						RoomActiveActorArray[i].Add(
							InGameLevel->GetWorld()->SpawnActor<ARoomActiveActor>(
								ARoomActiveActor::StaticClass(),
								FTransform(FRotator(0, 90, 0),
									RoadArray[i].Data[fiCount].V1RFF,
									FVector(1, 1, 1))));

						RoomActiveActorArray[i][RoomActiveActorArray[i].Num() - 1]->SetRoomNumber(RoadArray[i].Data[fiCount].V1);
					}
				}

				if (RoadArray[i].Data[fiCount].X == V2RF.X)
				{
					float Rot = 0;
					if (RoadArray[i].Data[1].X > V2RF.X)
						Rot = 180;
					else
						Rot = 0;

					DoorArray.Add(InGameLevel->GetWorld()->SpawnActor<ARoomDoor>(
						ARoomDoor::StaticClass(),
						FTransform(FRotator(0, Rot, 0),
							V2RF,
							FVector(1, 1, 1))));

					if (CheckBossRoom(i, fiCount)) //보스방 세팅
						DoorArray[DoorArray.Num() - 1]->BossDoor();

					if (RoadArray[i].Data[fiCount].V2 != RoomArray.Num() - 1)
					{
						RoomActiveActorArray[i].Add(
							InGameLevel->GetWorld()->SpawnActor<ARoomActiveActor>(
								ARoomActiveActor::StaticClass(),
								FTransform(FRotator(0, 0, 0),
									RoadArray[i].Data[fiCount].V2RFF,
									FVector(1, 1, 1))));

						RoomActiveActorArray[i][RoomActiveActorArray[i].Num() - 1]->SetRoomNumber(RoadArray[i].Data[fiCount].V2);
					}
				}
				else if (RoadArray[i].Data[fiCount].Y == V2RF.Y)
				{
					float Rot = 0;
					if (RoadArray[i].Data[1].Y > V2RF.Y)
						Rot = -90;
					else
						Rot = 90;

					DoorArray.Add(InGameLevel->GetWorld()->SpawnActor<ARoomDoor>(
						ARoomDoor::StaticClass(),
						FTransform(FRotator(0, Rot, 0),
							V2RF,
							FVector(1, 1, 1))));

					if (CheckBossRoom(i, fiCount)) //보스방 세팅
						DoorArray[DoorArray.Num() - 1]->BossDoor();

					if (RoadArray[i].Data[fiCount].V2 != RoomArray.Num() - 1)
					{
						RoomActiveActorArray[i].Add(
							InGameLevel->GetWorld()->SpawnActor<ARoomActiveActor>(
								ARoomActiveActor::StaticClass(),
								FTransform(FRotator(0, 90, 0),
									RoadArray[i].Data[fiCount].V2RFF,
									FVector(1, 1, 1))));

						RoomActiveActorArray[i][RoomActiveActorArray[i].Num() - 1]->SetRoomNumber(RoadArray[i].Data[fiCount].V2);
					}
				}
			}
			else
			{
				if (RoadArray[i].Data[fiCount].X == V1RF.X)
				{
					float Rot = 0;
					if (RoadArray[i].Data[0].X > V1RF.X)
						Rot = 180;
					else
						Rot = 0;

					DoorArray.Add(InGameLevel->GetWorld()->SpawnActor<ARoomDoor>(
						ARoomDoor::StaticClass(),
						FTransform(FRotator(0, Rot, 0),
							V1RF,
							FVector(1, 1, 1))));

					if (CheckBossRoom(i, fiCount)) //보스방 세팅
						DoorArray[DoorArray.Num() - 1]->BossDoor();

					if (RoadArray[i].Data[fiCount].V1 != RoomArray.Num() - 1)
					{
						RoomActiveActorArray[i].Add(
							InGameLevel->GetWorld()->SpawnActor<ARoomActiveActor>(
								ARoomActiveActor::StaticClass(),
								FTransform(FRotator(0, 180, 0),
									RoadArray[i].Data[fiCount].V1RFF,
									FVector(1, 1, 1))));

						RoomActiveActorArray[i][RoomActiveActorArray[i].Num() - 1]->SetRoomNumber(RoadArray[i].Data[fiCount].V1);
					}
				}
				else if (RoadArray[i].Data[fiCount].Y == V1RF.Y)
				{
					float Rot = 0;
					if (RoadArray[i].Data[0].Y > V1RF.Y)
						Rot = -90;
					else
						Rot = 90;

					DoorArray.Add(InGameLevel->GetWorld()->SpawnActor<ARoomDoor>(
						ARoomDoor::StaticClass(),
						FTransform(FRotator(0, Rot, 0),
							V1RF,
							FVector(1, 1, 1))));

					if (CheckBossRoom(i, fiCount)) //보스방 세팅
						DoorArray[DoorArray.Num() - 1]->BossDoor();

					if (RoadArray[i].Data[fiCount].V1 != RoomArray.Num() - 1)
					{
						RoomActiveActorArray[i].Add(
							InGameLevel->GetWorld()->SpawnActor<ARoomActiveActor>(
								ARoomActiveActor::StaticClass(),
								FTransform(FRotator(0, -90, 0),
									RoadArray[i].Data[fiCount].V1RFF,
									FVector(1, 1, 1))));

						RoomActiveActorArray[i][RoomActiveActorArray[i].Num() - 1]->SetRoomNumber(RoadArray[i].Data[fiCount].V1);
					}
				}

				if (RoadArray[i].Data[fiCount].X == V2RF.X)
				{
					float Rot = 0;
					if (RoadArray[i].Data[0].X > V2RF.X)
						Rot = 180;
					else
						Rot = 0;

					DoorArray.Add(InGameLevel->GetWorld()->SpawnActor<ARoomDoor>(
						ARoomDoor::StaticClass(),
						FTransform(FRotator(0, Rot, 0),
							V2RF,
							FVector(1, 1, 1))));

					if (CheckBossRoom(i, fiCount)) //보스방 세팅
						DoorArray[DoorArray.Num() - 1]->BossDoor();

					if (RoadArray[i].Data[fiCount].V2 != RoomArray.Num() - 1)
					{
						RoomActiveActorArray[i].Add(
							InGameLevel->GetWorld()->SpawnActor<ARoomActiveActor>(
								ARoomActiveActor::StaticClass(),
								FTransform(FRotator(0, 180, 0),
									RoadArray[i].Data[fiCount].V2RFF,
									FVector(1, 1, 1))));

						RoomActiveActorArray[i][RoomActiveActorArray[i].Num() - 1]->SetRoomNumber(RoadArray[i].Data[fiCount].V2);
					}
				}
				else if (RoadArray[i].Data[fiCount].Y == V2RF.Y)
				{
					float Rot = 0;
					if (RoadArray[i].Data[0].Y > V2RF.Y)
						Rot = -90;
					else
						Rot = 90;

					DoorArray.Add(InGameLevel->GetWorld()->SpawnActor<ARoomDoor>(
						ARoomDoor::StaticClass(),
						FTransform(FRotator(0, Rot, 0),
							V2RF,
							FVector(1, 1, 1))));

					if (CheckBossRoom(i, fiCount)) //보스방 세팅
						DoorArray[DoorArray.Num() - 1]->BossDoor();

					if (RoadArray[i].Data[fiCount].V2 != RoomArray.Num() - 1)
					{
						RoomActiveActorArray[i].Add(
							InGameLevel->GetWorld()->SpawnActor<ARoomActiveActor>(
								ARoomActiveActor::StaticClass(),
								FTransform(FRotator(0, -90, 0),
									RoadArray[i].Data[fiCount].V2RFF,
									FVector(1, 1, 1))));

						RoomActiveActorArray[i][RoomActiveActorArray[i].Num() - 1]->SetRoomNumber(RoadArray[i].Data[fiCount].V2);
					}
				}
			}
		}
	}

	//방마다 몬스터 배치하는 코드
	for (int i = 0; i < RoomArray.Num() - 1; i++)
	{
		MonsterArray.Add(TArray<FMonsterDataTableRow>());
		int RoomStrongScore = 0;

		if (GameInstance->RoomTemplateData[iStage][RoomArray[i].RoomNumber].IsMonsterRoom)
		{
			UE_LOG(LogTemp, Log, TEXT("Spawn Nomal Monster"));

			while (RoomStrongScore < GameInstance->RoomTemplateData[iStage][RoomArray[i].RoomNumber].RoomMonsterMaxScore)
			{
				int randNum = rand() % GameInstance->MonsterData[iStage].Num();
				RoomStrongScore += GameInstance->MonsterData[iStage][randNum].StrongScore;

				if (RoomStrongScore > GameInstance->RoomTemplateData[iStage][RoomArray[i].RoomNumber].RoomMonsterMaxScore)
					RoomStrongScore -= GameInstance->MonsterData[iStage][randNum].StrongScore;
				else
					MonsterArray[i].Add(GameInstance->MonsterData[iStage][randNum]);
			}
		}
	}

	UE_LOG(LogTemp, Log, TEXT("END"));
}

void MapGeneratorSys::DeleteMap()
{
	for (int i = 0; i < TemplateActorArray.Num(); i++)
	{
		TemplateActorArray[i]->DestroyRoom();
		TemplateActorArray[i]->Destroy();
	}

	for (int i = 0; i < RoadTemplateActorArray.Num(); i++)
		RoadTemplateActorArray[i]->Destroy();

	for (int i = 0; i < DoorArray.Num(); i++)
		DoorArray[i]->Destroy();

	for (int i = 0; i < RoomActiveActorArray.Num(); i++)
		for (int j = 0; j < RoomActiveActorArray[i].Num(); j++)
			RoomActiveActorArray[i][j]->Destroy();

	TemplateActorArray.Empty();
	RoadTemplateActorArray.Empty();
	RoomArray.Empty();
	RoadArray.Empty();
	DoorArray.Empty();
	RoomActiveActorArray.Empty();
	MonsterArray.Empty();
}

void MapGeneratorSys::RoomActiveActor(int _RoomNumber)
{
	if (RoomActiveActorArray[_RoomNumber].Num() > 0)
	{
		if (GameInstance->RoomTemplateData[iStage][RoomArray[_RoomNumber].RoomNumber].IsMonsterRoom)
		{
			GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Blue, TEXT("Active Room"));


			for (int i = 0; i < MonsterArray[_RoomNumber].Num();)
			{
				int iNum = GameInstance->RoomTemplateData[iStage][RoomArray[_RoomNumber].RoomNumber].Template->ActorArr.Num();

				int randNum = rand() % iNum;

				UE_LOG(LogTemp, Log, TEXT("RoomActiveActor SpawnMonster %s"), *GameInstance->RoomTemplateData[iStage][RoomArray[_RoomNumber].RoomNumber].Template->ActorArr[randNum].MeshData);

				if (GameInstance->RoomTemplateData[iStage][RoomArray[_RoomNumber].RoomNumber].Template->ActorArr[randNum].MonsterSpawn)
				{
					FVector Lo = GameInstance->RoomTemplateData[iStage][RoomArray[_RoomNumber].RoomNumber].Template->ActorArr[randNum].MeshTransform.GetLocation();
					FVector Lo1 = FVector(RoomArray[_RoomNumber].X, RoomArray[_RoomNumber].Y, 100);

					switch (MonsterArray[_RoomNumber][i].AttackType)
					{
					case E_MonsterAttackType::M_BumpType:
						InGameLevel->GetWorld()->SpawnActor<AMonsterActor>(
							ABumpTypeMonster::StaticClass(),
							FTransform(FRotator(0, 0, 0),
								FVector(Lo.X + Lo1.X, Lo.Y + Lo1.Y, 100),
								FVector(1, 1, 1)))
							->SetData(MonsterArray[_RoomNumber][i]);
						break;

					case E_MonsterAttackType::M_MeleeType:
						InGameLevel->GetWorld()->SpawnActor<AMonsterActor>(
							AMeleeType::StaticClass(),
							FTransform(FRotator(0, 0, 0),
								FVector(Lo.X + Lo1.X, Lo.Y + Lo1.Y, 100),
								FVector(1, 1, 1)))
							->SetData(MonsterArray[_RoomNumber][i]);
						break;

					case E_MonsterAttackType::M_StandOffType:
						InGameLevel->GetWorld()->SpawnActor<AMonsterActor>(
							AStandOffTypeMonster::StaticClass(),
							FTransform(FRotator(0, 0, 0),
								FVector(Lo.X + Lo1.X, Lo.Y + Lo1.Y, 100),
								FVector(1, 1, 1)))
							->SetData(MonsterArray[_RoomNumber][i]);
						break;
					}

					iMonsterCount++;


					RoomActiveActorArray[_RoomNumber].Empty();
					UE_LOG(LogTemp, Log, TEXT("RoomActiveActor Test %d"), _RoomNumber);

					InGameLevel->UpDateNavMesh(
						FVector(RoomArray[_RoomNumber].SX, RoomArray[_RoomNumber].SY, 100),
						FVector(
							RoomArray[_RoomNumber].X + RoomArray[_RoomNumber].SX / 2,
							RoomArray[_RoomNumber].Y + RoomArray[_RoomNumber].SY / 2, -10));

					for (int ii = 0; ii < DoorArray.Num(); ii++)
						DoorArray[ii]->ActiveDoor();

					i++;
				}
			}
		}
		else if (GameInstance->RoomTemplateData[iStage][RoomArray[_RoomNumber].RoomNumber].IsBossRoom)
		{
			//보스 몬스터 스폰
			GEngine->AddOnScreenDebugMessage(-1, 60, FColor::Red, TEXT("Active Boos Room"));

			FVector Lo = FVector(
				RoomArray[_RoomNumber].X + RoomArray[_RoomNumber].SX / 2,
				RoomArray[_RoomNumber].Y + RoomArray[_RoomNumber].SY / 2, -10);

			int BossRandom = rand() % GameInstance->BossData[iStage].Num();

			InGameLevel->GetWorld()->SpawnActor<ABossActor>(
				GameInstance->BossData[iStage][BossRandom].Class,
				FTransform(FRotator(0, 0, 0),
					FVector(Lo.X, Lo.Y, GameInstance->BossData[iStage][BossRandom].SpawnHeight),
					FVector(1, 1, 1)));

			RoomActiveActorArray[_RoomNumber].Empty();

			InGameLevel->UpDateNavMesh(
				FVector(RoomArray[_RoomNumber].SX, RoomArray[_RoomNumber].SY, 100),
				FVector(
					RoomArray[_RoomNumber].X + RoomArray[_RoomNumber].SX / 2,
					RoomArray[_RoomNumber].Y + RoomArray[_RoomNumber].SY / 2, -10));

			for (int ii = 0; ii < DoorArray.Num(); ii++)
				DoorArray[ii]->ActiveDoor();
		}
	}
}

void MapGeneratorSys::RoomInActiveActor()
{
	for (int i = 0; i < DoorArray.Num(); i++)
		DoorArray[i]->InActiveDoor();
}

void MapGeneratorSys::DestroyMonster()
{
	iMonsterCount -= 1;

	if (iMonsterCount <= 0)
	{
		RoomInActiveActor();
		iMonsterCount = 0;
	}
}

void MapGeneratorSys::DestroyBoss()
{
	RoomInActiveActor();
}

TArray<FRoomData> MapGeneratorSys::GetRoomArray()
{
	return RoomArray;
}

TArray<FRoadData> MapGeneratorSys::GetRoadArray()
{
	return RoadArray;
}

TArray<ARoomDoor*> MapGeneratorSys::GetDoorArray()
{
	return DoorArray;
}

FALGraph MapGeneratorSys::GetGraph()
{
	return graph;
}

FVector MapGeneratorSys::GetStartLo()
{
	FVector Lo;
	Lo.X = RoomArray[RoomArray.Num() - 1].X + (RoomArray[RoomArray.Num() - 1].SX / 2);
	Lo.Y = RoomArray[RoomArray.Num() - 1].Y + (RoomArray[RoomArray.Num() - 1].SY / 2);
	Lo.Z = 100;

	return Lo;
}

int MapGeneratorSys::GetMaxDisIndex()
{
	return MaxDisIndex;
}

bool MapGeneratorSys::IntersectLine(const FVector& _SP1, const FVector& _EP1, const FVector& _SP2, const FVector& _EP2)
{
	double den = (_EP2.Y - _SP2.Y) * (_EP1.X - _SP1.X) - (_EP2.X - _SP2.X) * (_EP1.Y - _SP1.Y);
	if (den == 0) return false;

	double p1 = (_EP2.X - _SP2.X) * (_SP1.Y - _SP2.Y) - (_EP2.Y - _SP2.Y) * (_SP1.X - _SP2.X);
	double p2 = (_EP1.X - _SP1.X) * (_SP1.Y - _SP2.Y) - (_EP1.Y - _SP1.Y) * (_SP1.X - _SP2.X);

	double op1 = p1 / den;
	double op2 = p2 / den;

	if (op1 < 0.0 || op1 > 1.0 || op2 < 0.0 || op2 > 1.0)
		return false;
	if (p1 == 0 && p2 == 0)
		return false;

	return true;
}

bool MapGeneratorSys::CheckBossRoom(int i, int fiCount)
{
	if (RoadArray[i].Data[fiCount].V1 == RoomArray.Num() - 2 ||
		RoadArray[i].Data[fiCount].V2 == RoomArray.Num() - 2)
		return true;

	return false;
}
