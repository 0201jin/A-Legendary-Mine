// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/DataTable.h"
#include "Engine.h"
#include "MyCustomAsset.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponDataTable.generated.h"

UENUM(BlueprintType)
enum class E_WeaponType : uint8
{
	W_OneHand_SWORD UMETA(DisplayName = "OneHand SWORD")
};

USTRUCT(BlueprintType)
struct FWeaponDataTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 ItemCode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		E_WeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		UStaticMesh * WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		UStaticMesh* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		UTexture* ItemImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable", meta = (MultiLine = true))
		FText Explain;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 HitBoxX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		int32 HitBoxY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDataTable")
		float AttackSpeed;
};

UCLASS()
class ALEGENDARYMINE_API AWeaponDataTable : public AActor
{
	GENERATED_BODY()
};
