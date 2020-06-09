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

	static ConstructorHelpers::FObjectFinder<UDataTable> BuffData(TEXT("DataTable'/Game/Card/DataTable/BuffDataTable.BuffDataTable'"));
	BuffDataTable = BuffData.Object;

	static ConstructorHelpers::FObjectFinder<UDataTable> DeBuffData(TEXT("DataTable'/Game/Card/DataTable/DeBuffDataTable.DeBuffDataTable'"));
	DeBuffDataTable = DeBuffData.Object;
}

void UMyGameInstance::Init()
{
	Super::Init();

	//방 템플릿 데이터 테이블
	for (int i = 0; i < TemplateDataTable.Num(); i++)
	{
		RoomTemplateData.Add(TArray<FTemplateDataTableRow>());

		FString RoomTemplateRSContextString;
		TArray<FName> RoomTemplateRSRowNames;
		RoomTemplateRSRowNames = TemplateDataTable[i]->GetRowNames();

		for (auto& name : RoomTemplateRSRowNames)
		{
			FTemplateDataTableRow* RoomTemplateRSTableRow = TemplateDataTable[i]->FindRow<FTemplateDataTableRow>(name, RoomTemplateRSContextString);

			if (RoomTemplateRSTableRow)
			{
				RoomTemplateData[i].Add(*RoomTemplateRSTableRow);

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
		MonsterData.Add(TArray<FMonsterDataTableRow>());

		FString MonsterTemplateRSContextString;
		TArray<FName> MonsterTemplateRSRowNames;
		MonsterTemplateRSRowNames = MonsterDataTable[i]->GetRowNames();

		for (auto& name : MonsterTemplateRSRowNames)
		{
			FMonsterDataTableRow* MonsterTemplateRSTableRow = MonsterDataTable[i]->FindRow<FMonsterDataTableRow>(name, MonsterTemplateRSContextString);

			if (MonsterTemplateRSTableRow)
			{
				MonsterData[i].Add(*MonsterTemplateRSTableRow);
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

	//버프 데이터 테이블
	FString BuffRSContextString;
	TArray<FName> BuffRSRowNames;
	BuffRSRowNames = BuffDataTable->GetRowNames();

	for (auto& name : BuffRSRowNames)
	{
		FCardDataTableRow* BuffRSTableRow = BuffDataTable->FindRow<FCardDataTableRow>(name, BuffRSContextString);

		if (BuffRSTableRow)
		{
			BuffData.Add(*BuffRSTableRow);
		}
	}

	//디버프 데이터 테이블
	FString DeBuffRSContextString;
	TArray<FName> DeBuffRSRowNames;
	DeBuffRSRowNames = DeBuffDataTable->GetRowNames();

	for (auto& name : DeBuffRSRowNames)
	{
		FCardDataTableRow* DeBuffRSTableRow = DeBuffDataTable->FindRow<FCardDataTableRow>(name, DeBuffRSContextString);

		if (DeBuffRSTableRow)
		{
			DeBuffData.Add(*DeBuffRSTableRow);
		}
	}
}
