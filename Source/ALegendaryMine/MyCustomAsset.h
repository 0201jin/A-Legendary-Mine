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
		TArray<FInstancedStaticMeshInstanceData> ActorData; //Transfrom 저장

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
		int16 Index = -1; //ISMData를 가르키는 Index
};

USTRUCT(Atomic)
struct FAData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FString MeshData;

	UPROPERTY()
		FTransform MeshTransform;
};

USTRUCT(Atomic)
struct FActorData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
		FString MeshData;

	UPROPERTY()
		int16 ArrayIndex = 0;
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

	UPROPERTY()
		TArray<FAData> ActorArr;

	UPROPERTY()
		TArray<FActorData> ActorDataArr;

	UPROPERTY()
		int32 SX;

	UPROPERTY()
		int32 SY;

	UPROPERTY()
		bool IsBossRoom;

	bool bSelectActor = false;
	int RoomMaxScoreSize = 0;

	int16 MeshDataIndex = 0; //MeshDataArr의 Index를 가르킴
	int16 ActorDataIndex = 0;
};
