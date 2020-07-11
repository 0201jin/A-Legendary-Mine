// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "MyCustomAsset.h"
#include "Struct/MapGeneratorStruct.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomTemplateActor.generated.h"

UCLASS()
class ALEGENDARYMINE_API ARoomTemplateActor : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ARoomTemplateActor();

	void SetAsset(UMyCustomAsset* _MyCustomAsset);
	void CreateRoad(FVector _RoadLo, FVector _Lo);
	void DestroyRoom();

	void SetRoadMeshData(FRoadMeshData _MeshData);
	void CreateNewRoad() { iCount = 1; bFindBlackWall = false; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UMyCustomAsset* MyCustomAsset;

	UInstancedStaticMeshComponent* Walls;
	TArray<UInstancedStaticMeshComponent*> InstanceActor;
	TArray<AActor*> ActorArr;

	int iCount = 1;
	bool bFindBlackWall = false;
};
