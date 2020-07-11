// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/DataTable.h"
#include "Engine.h"
#include "MyCustomAsset.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TemplateDataTable.generated.h"

USTRUCT(BlueprintType)
struct FTemplateDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		UMyCustomAsset * Template;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		bool IsBossRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		bool IsMonsterRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		bool IsTreasureRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		bool IsNPCRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		bool IsShopRoom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 RoomMonsterMaxScore;
};

USTRUCT(BlueprintType)
struct FStageSizeDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 StageSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 TreasureRoomNumer;
};

UCLASS()
class ALEGENDARYMINE_API ATemplateDataTable : public AActor
{
	GENERATED_BODY()
};
