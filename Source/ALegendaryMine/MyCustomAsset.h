// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MyCustomAsset.generated.h"

/**
 * 
 */
USTRUCT(Atomic)
struct FISMData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		TArray<FInstancedStaticMeshInstanceData> ActorData; //Transfrom ����

	UPROPERTY()
		FString Meshdata;
};

USTRUCT(Atomic)
struct FMeshData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FString MeshData;

	UPROPERTY()
		int16 ArrayIndex = 0;

	UPROPERTY()
		int16 Index = -1; //ISMData�� ����Ű�� Index
};

UCLASS()
class ALEGENDARYMINE_API UMyCustomAsset : public UObject
{
	GENERATED_BODY()

public:
	UMyCustomAsset();

	UPROPERTY()
		TArray<FISMData> ActorData;

	UPROPERTY()
		TArray<FMeshData> MeshDataArr;

	int16 MeshDataIndex = 0; //MeshDataArr�� Index�� ����Ŵ
};
