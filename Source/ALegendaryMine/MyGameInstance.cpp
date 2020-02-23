// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Engine.h"

#include "DataTable/TemplateDataTable.h"

UMyGameInstance::UMyGameInstance()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> Template1(TEXT("DataTable'/Game/Template/TemplateDataTable/Stage1.Stage1'"));
	TemplateDataTable.Add(Template1.Object);
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
}
