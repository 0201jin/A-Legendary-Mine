// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomDoor.generated.h"

UCLASS()
class ALEGENDARYMINE_API ARoomDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoomDoor();

	void ActiveDoor();
	void InActiveDoor();
	void BossDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UStaticMeshComponent* Walls_0;
	UStaticMeshComponent* Walls_1;
	UStaticMeshComponent* Walls_2;

	UStaticMeshComponent* BossDoorFrameMesh;
	UStaticMeshComponent* BossDoorMesh;

	bool bBossDoor = false;
};
