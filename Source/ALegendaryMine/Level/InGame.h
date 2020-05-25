// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "System/MapGeneratorSys/MapGeneratorSys.h"
#include "MyGameInstance.h"

#include "Engine/BrushBuilder.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "InGame.generated.h"

/**
 *
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpDateNavMeshSize, FVector, Size, FVector, Location);

UCLASS()
class ALEGENDARYMINE_API AInGame : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	AInGame();
	~AInGame();

	void GenerateMap();
	void UpDateNavMesh(FVector _Size, FVector _Location);

	UMyGameInstance* GetMyGameInstance();
	MapGeneratorSys* GetMapgen() { return Mapgen; }

protected:
	virtual void BeginPlay();

protected:
	MapGeneratorSys* Mapgen;
	UMyGameInstance* MyGameInstance;

	UPROPERTY(BluePrintAssignable, Category = "UpDateNavMesh")
		FUpDateNavMeshSize UpdateNavMeshSize;

public:
	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void SpawnMonster(int _Stage, int _Number);
};
