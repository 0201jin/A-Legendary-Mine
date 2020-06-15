// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/DataTable.h"
#include "Engine.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossDataTable.generated.h"

USTRUCT(BlueprintType)
struct FBossDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		UClass* Class;
};

UCLASS()
class ALEGENDARYMINE_API ABossDataTable : public AActor
{
	GENERATED_BODY()
};
