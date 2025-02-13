// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "System/MapGeneratorSys/MapGeneratorSys.h"
#include "MyGameInstance.h"
#include "HUD/MyHUD.h"
#include "CardSystem/CardSystem.h"

#include "Engine/BrushBuilder.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "InGame.generated.h"

/**
 *
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpDateNavMeshSize, FVector, Size, FVector, Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateCamera);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateCameraCharacter);

UCLASS()
class ALEGENDARYMINE_API AInGame : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	AInGame();
	~AInGame();

	void GenerateMap();
	void UpDateNavMesh(FVector _Size, FVector _Location);

	void ShuffleCard();
	void CameraToCharacter();

	UMyGameInstance* GetMyGameInstance();
	MapGeneratorSys* GetMapgen() { return Mapgen; }

protected:
	virtual void BeginPlay();

protected:
	MapGeneratorSys* Mapgen;
	UMyGameInstance* MyGameInstance;
	ACardSystem* CardSystem;
	AMyHUD* MyHUD;

	UPROPERTY(BluePrintAssignable, Category = "UpDateNavMesh")
		FUpDateNavMeshSize UpdateNavMeshSize;

	UPROPERTY(BluePrintAssignable, Category = "UpdateCamera")
		FUpdateCamera UpdateCamera;

	UPROPERTY(BluePrintAssignable, Category = "UpdateCamera")
		FUpdateCameraCharacter UpdateCameraCharacter;

public: //명령어 추후 반드시 지울 것.
	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void SpawnMonster(int _Stage, int _Number);

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void TPRoom(int _RoomNumber);
};
