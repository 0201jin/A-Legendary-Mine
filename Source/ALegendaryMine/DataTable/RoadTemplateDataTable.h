// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/DataTable.h"
#include "Engine.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoadTemplateDataTable.generated.h"

USTRUCT(BlueprintType)
struct FRoadTemplateDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		UStaticMesh * Floor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		UStaticMesh * Walls;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		UStaticMesh * Doors;
};

UCLASS()
class ALEGENDARYMINE_API ARoadTemplateDataTable : public AActor
{
	GENERATED_BODY()
};
