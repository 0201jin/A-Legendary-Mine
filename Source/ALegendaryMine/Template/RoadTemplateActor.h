// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Struct/MapGeneratorStruct.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoadTemplateActor.generated.h"

UCLASS()
class ALEGENDARYMINE_API ARoadTemplateActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoadTemplateActor();

	void SetRoadMeshData(FRoadMeshData _MeshData, FRoadData _RoadData);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void CreateRoad(FVector _Road, FVector _Lo);

protected:
	UInstancedStaticMeshComponent* Floor;
	UInstancedStaticMeshComponent* Walls;
	UInstancedStaticMeshComponent* Door;
	UInstancedStaticMeshComponent* BlackWalls;

	FRoadMeshData MeshData;
	FRoadData RoadData;
};
