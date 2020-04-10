// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/DataTable.h"
#include "Engine.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MonsterDataTable.generated.h"

UENUM(BlueprintType)
enum class E_MonsterAttackType : uint8
{
	M_BumpType UMETA(DisplayName = "Bump Type"),
	M_MeleeType UMETA(DisplayName = "Melee Type")
};

USTRUCT(BlueprintType)
struct FMonsterDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		USkeletalMesh* Skeleton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		UMaterialInstance* Material;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		UAnimBlueprint* Animation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		UStaticMesh* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		E_MonsterAttackType AttackType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 AttackDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 StrongScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 DropMoney;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Height;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Z;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		FVector Size;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		FVector HitBoxSize;
};

UCLASS()
class ALEGENDARYMINE_API AMonsterDataTable : public AActor
{
	GENERATED_BODY()
};
