// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/DataTable.h"
#include "Engine.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterDataTable.generated.h"

USTRUCT(BlueprintType)
struct FMonsterDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		USkeletalMesh* Skeleton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		UAnimBlueprint* Animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		UStaticMesh* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 StrongScore;
};

UCLASS()
class ALEGENDARYMINE_API AMonsterDataTable : public AActor
{
	GENERATED_BODY()
};
