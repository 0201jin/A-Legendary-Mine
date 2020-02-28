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
}

void UMyGameInstance::Init()
{
	Super::Init();

	for (int i = 0; i < TemplateDataTable.Num(); i++)
	{
		RoomTemplateData.Add(TArray<class UMyCustomAsset*>());

		FString RoomTemplateRSContextString;
		TArray<FName> RoomTemplateRSRowNames;
		RoomTemplateRSRowNames = TemplateDataTable[i]->GetRowNames();

		for (auto& name : RoomTemplateRSRowNames)
		{
			FTemplateDataTableRow * RoomTemplateRSTableRow = TemplateDataTable[i]->FindRow<FTemplateDataTableRow>(name, RoomTemplateRSContextString);

			if (RoomTemplateRSTableRow)
			{
				RoomTemplateRSTableRow->Template->IsBossRoom = RoomTemplateRSTableRow->IsBossRoom;
				RoomTemplateData[i].Add(RoomTemplateRSTableRow->Template);

				UE_LOG(LogTemp, Log, TEXT("RoomTemplate"));
			}
		}
	}

	for (int i = 0; i < RoadTemplateDataTable.Num(); i++)
	{
		RoadTemplateData.Add(TArray<FRoadMeshData>());

		FString RoadTemplateRSContextString;
		TArray<FName> RoadTemplateRSRowNames;
		RoadTemplateRSRowNames = RoadTemplateDataTable[i]->GetRowNames();

		for (auto& name : RoadTemplateRSRowNames)
		{
			FRoadTemplateDataTableRow * RoadTemplateRSTableRow = RoadTemplateDataTable[i]->FindRow<FRoadTemplateDataTableRow>(name, RoadTemplateRSContextString);

			if (RoadTemplateRSTableRow)
			{
				FRoadMeshData Data;
				Data.Floor = RoadTemplateRSTableRow->Floor;
				Data.Walls = RoadTemplateRSTableRow->Walls;
				Data.Doors = RoadTemplateRSTableRow->Doors;

				RoadTemplateData[i].Add(Data);

				UE_LOG(LogTemp, Log, TEXT("RoadTemplate"));
			}
		}
	}
}
