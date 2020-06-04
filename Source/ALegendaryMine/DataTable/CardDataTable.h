// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/DataTable.h"
#include "Engine.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardDataTable.generated.h"

UENUM(BlueprintType)
enum class E_CardType : uint8
{
	C_EmptyType UMETA(DisplayName = "Empty Type"),
	C_SpawnType UMETA(DisplayName = "Spawn Type"),
	C_TickType UMETA(DisplayName = "Tick Type"),
	C_StatType UMETA(DisplayName = "Stat Type"),
	C_AddDamageType UMETA(DisplayName = "Add Damage Type"),
	C_AvoidType UMETA(DisplayName = "Avoid Type"),
};

UENUM(BlueprintType)
enum class E_C_SpawnType : uint8
{
	C_S_GhostType UMETA(DisplayName = "Ghost Type"),
	C_S_ObjectType UMETA(DisplayName = "Object Type"),
};

UENUM(BlueprintType)
enum class E_C_EffectType : uint8
{
	C_E_DamageType UMETA(DisplayName = "Damage Type"),
	C_E_MoneyType UMETA(DisplayName = "Money Type"),
};

USTRUCT(BlueprintType)
struct FCardDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		E_CardType CardType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		FText Explain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		E_C_SpawnType SpawnType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		bool Loop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		float LoopTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		E_C_EffectType EffectType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		float EffectValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 AddDamaged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 AVoid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 AD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 AS;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 MS;
};

UCLASS()
class ALEGENDARYMINE_API ACardDataTable : public AActor
{
	GENERATED_BODY()
};
