// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Engine.h"

#include "DataTable/TemplateDataTable.h"

UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Template1(TEXT("DataTable'/Game/Template/TemplateDataTable/Stage1.Stage1'"));
	TemplateDataTable.Add(Template1.Object);

	static ConstructorHelpers::FObjectFinder<UDataTable> RoadTemplate1(TEXT("DataTable'/Game/Template/TemplateDataTable/Road_Stage1.Road_Stage1'"));
	RoadTemplateDataTable.Add(RoadTemplate1.Object);

	static ConstructorHelpers::FObjectFinder<UDataTable> MonsterData1(TEXT("DataTable'/Game/Template/TemplateDataTable/Monster_Stage1.Monster_Stage1'"));
	MonsterDataTable.Add(MonsterData1.Object);

	static ConstructorHelpers::FObjectFinder<UDataTable> StageSizeData(TEXT("DataTable'/Game/Template/TemplateDataTable/StageSize/StageSizeDataTable.StageSizeDataTable'"));
	StageSizeDataTable = StageSizeData.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> WeaponData(TEXT("DataTable'/Game/Weapon/WeaponDataTable.WeaponDataTable'"));
	WeaponDataTable = WeaponData.Object;
}

void UMyGameInstance::Init()
{
	Super::Init();

	//방 템플릿 데이터 테이블
	for (int i = 0; i < TemplateDataTable.Num(); i++)
	{
		RoomTemplateData.Add(TArray<class UMyCustomAsset*>());

		FString RoomTemplateRSContextString;
		TArray<FName> RoomTemplateRSRowNames;
		RoomTemplateRSRowNames = TemplateDataTable[i]->GetRowNames();

		for (auto& name : RoomTemplateRSRowNames)
		{
			FTemplateDataTableRow* RoomTemplateRSTableRow = TemplateDataTable[i]->FindRow<FTemplateDataTableRow>(name, RoomTemplateRSContextString);

			if (RoomTemplateRSTableRow)
			{
				RoomTemplateRSTableRow->Template->IsBossRoom = RoomTemplateRSTableRow->IsBossRoom;
				RoomTemplateRSTableRow->Template->RoomMaxScoreSize = RoomTemplateRSTableRow->RoomMonsterMaxScore;
				RoomTemplateData[i].Add(RoomTemplateRSTableRow->Template);

				UE_LOG(LogTemp, Log, TEXT("RoomTemplate Load"));
			}
		}
	}

	//길 템플릿 데이터 테이블
	for (int i = 0; i < RoadTemplateDataTable.Num(); i++)
	{
		RoadTemplateData.Add(TArray<FRoadMeshData>());

		FString RoadTemplateRSContextString;
		TArray<FName> RoadTemplateRSRowNames;
		RoadTemplateRSRowNames = RoadTemplateDataTable[i]->GetRowNames();

		for (auto& name : RoadTemplateRSRowNames)
		{
			FRoadTemplateDataTableRow* RoadTemplateRSTableRow = RoadTemplateDataTable[i]->FindRow<FRoadTemplateDataTableRow>(name, RoadTemplateRSContextString);

			if (RoadTemplateRSTableRow)
			{
				FRoadMeshData Data;
				Data.Floor = RoadTemplateRSTableRow->Floor;
				Data.Walls = RoadTemplateRSTableRow->Walls;
				Data.Doors = RoadTemplateRSTableRow->Doors;

				RoadTemplateData[i].Add(Data);

				UE_LOG(LogTemp, Log, TEXT("RoadTemplate Load"));
			}
		}
	}

	//몬스터 데이터 테이블
	for (int i = 0; i < MonsterDataTable.Num(); i++)
	{
		MonsterData.Add(TArray<FMonsterData>());

		FString MonsterTemplateRSContextString;
		TArray<FName> MonsterTemplateRSRowNames;
		MonsterTemplateRSRowNames = MonsterDataTable[i]->GetRowNames();

		for (auto& name : MonsterTemplateRSRowNames)
		{
			FMonsterDataTableRow* MonsterTemplateRSTableRow = MonsterDataTable[i]->FindRow<FMonsterDataTableRow>(name, MonsterTemplateRSContextString);

			if (MonsterTemplateRSTableRow)
			{
				FMonsterData Data;
				Data.Animation = MonsterTemplateRSTableRow->Animation;
				Data.AttackDamage = MonsterTemplateRSTableRow->AttackDamage;
				Data.AttackSpeed = MonsterTemplateRSTableRow->AttackSpeed;
				Data.DropMoney = MonsterTemplateRSTableRow->DropMoney;
				Data.ProjectileMesh = MonsterTemplateRSTableRow->ProjectileMesh;
				Data.Skeleton = MonsterTemplateRSTableRow->Skeleton;
				Data.Speed = MonsterTemplateRSTableRow->Speed;
				Data.StrongScore = MonsterTemplateRSTableRow->StrongScore;
				Data.Health = MonsterTemplateRSTableRow->Health;
				Data.Height = MonsterTemplateRSTableRow->Height;
				Data.Radius = MonsterTemplateRSTableRow->Radius;
				Data.Z = MonsterTemplateRSTableRow->Z;

				MonsterData[i].Add(Data);

				if (Data.Animation != nullptr)
					UE_LOG(LogTemp, Log, TEXT("MonsterData Load"));
			}
		}
	}

	//스테이지 사이즈 데이터 테이블
	FString StageSizeRSContextString;
	TArray<FName> StageSizeRSRowNames;
	StageSizeRSRowNames = StageSizeDataTable->GetRowNames();

	for (auto& name : StageSizeRSRowNames)
	{
		FStageSizeDataTableRow* StageSizeRSTableRow = StageSizeDataTable->FindRow<FStageSizeDataTableRow>(name, StageSizeRSContextString);

		if (StageSizeRSTableRow)
		{
			StageSizeData.Add(*StageSizeRSTableRow);
		}
	}

	//무기 데이터 테이블
	FString WeaponRSContextString;
	TArray<FName> WeaponRSRowNames;
	WeaponRSRowNames = WeaponDataTable->GetRowNames();

	for (auto& name : WeaponRSRowNames)
	{
		FWeaponDataTableRow* WeaponRSTableRow = WeaponDataTable->FindRow<FWeaponDataTableRow>(name, WeaponRSContextString);

		if (WeaponRSTableRow)
		{
			WeaponData.Add(*WeaponRSTableRow);
		}
	}
}
