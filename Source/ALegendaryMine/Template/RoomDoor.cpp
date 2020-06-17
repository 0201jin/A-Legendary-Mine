// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomDoor.h"

// Sets default values
ARoomDoor::ARoomDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bHidden = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Steel_Barred_0(TEXT("StaticMesh'/Game/Template/Steel_Barred__1fbx.Steel_Barred__1fbx'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Steel_Barred_1(TEXT("StaticMesh'/Game/Template/Steel_Barred.Steel_Barred'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Boss_Door_Frame(TEXT("StaticMesh'/Game/Template/BossDoor/DoorFame/DoorFrame.DoorFrame'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Boss_Door(TEXT("StaticMesh'/Game/Template/BossDoor/Door/Door.Door'"));
	
	Walls_0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh0"));
	Walls_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh1"));
	Walls_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh2"));
	BossDoorFrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BossDoorFrame"));
	BossDoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BossDoor"));

	Walls_0->AttachTo(RootComponent);
	Walls_0->SetStaticMesh(Steel_Barred_0.Object);
	Walls_0->SetRelativeScale3D(FVector(1, 1, 2));
	Walls_0->SetRelativeLocation(FVector(0, 0, -200));

	Walls_1->AttachTo(RootComponent);
	Walls_1->SetStaticMesh(Steel_Barred_1.Object);
	Walls_1->SetRelativeLocation(FVector(0, 0, -100));

	Walls_2->AttachTo(RootComponent);
	Walls_2->SetStaticMesh(Steel_Barred_1.Object);
	Walls_2->SetRelativeLocation(FVector(0, 0, -190));

	BossDoorFrameMesh->AttachTo(RootComponent);
	BossDoorFrameMesh->SetStaticMesh(Boss_Door_Frame.Object);
	BossDoorFrameMesh->SetRelativeScale3D(FVector(1, 1, 1.25));

	BossDoorMesh->AttachTo(RootComponent);
	BossDoorMesh->SetStaticMesh(Boss_Door.Object);
	BossDoorMesh->SetRelativeScale3D(FVector(1, 1, 1.25));
}

void ARoomDoor::ActiveDoor()
{
	if (!bBossDoor)
	{
		Walls_0->SetHiddenInGame(false);
		Walls_1->SetHiddenInGame(false);
		Walls_2->SetHiddenInGame(false);

		Walls_0->SetRelativeLocation(FVector(0, 0, 0));
		Walls_1->SetRelativeLocation(FVector(0, 0, 0));
		Walls_2->SetRelativeLocation(FVector(0, 0, 90));
	}
	else
	{
		BossDoorMesh->SetHiddenInGame(false);
		BossDoorMesh->SetRelativeLocation(FVector(-10, 0, 0));
	}
}

void ARoomDoor::InActiveDoor()
{
	if (!bBossDoor)
	{
		Walls_0->SetHiddenInGame(true);
		Walls_1->SetHiddenInGame(true);
		Walls_2->SetHiddenInGame(true);

		Walls_0->SetRelativeLocation(FVector(0, 0, -200));
		Walls_1->SetRelativeLocation(FVector(0, 0, -100));
		Walls_2->SetRelativeLocation(FVector(0, 0, -190));
	}
	else
	{
		BossDoorMesh->SetHiddenInGame(true);
		BossDoorMesh->SetRelativeLocation(FVector(0, 0, -400));
	}
}

void ARoomDoor::BossDoor()
{
	BossDoorFrameMesh->SetHiddenInGame(false);
	BossDoorFrameMesh->SetRelativeLocation(FVector(0, 0, 0));

	bBossDoor = true;
}

void ARoomDoor::BeginPlay()
{
	Super::BeginPlay();

	BossDoorMesh->SetHiddenInGame(true);
	BossDoorMesh->SetRelativeLocation(FVector(0, 0, -400));

	Walls_0->SetHiddenInGame(true);
	Walls_1->SetHiddenInGame(true);
	Walls_2->SetHiddenInGame(true);
	BossDoorFrameMesh->SetHiddenInGame(true);
	BossDoorFrameMesh->SetRelativeLocation(FVector(0, 0, -400));
}