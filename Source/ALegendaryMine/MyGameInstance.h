// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DataTable/TemplateDataTable.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALEGENDARYMINE_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UMyGameInstance();

protected:
	virtual void Init();

protected:
	TArray<class UDataTable *> TemplateDataTable;

public:
	TArray<TArray<class UMyCustomAsset*>> RoomTemplateData;
};
