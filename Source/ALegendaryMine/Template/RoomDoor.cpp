// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomDoor.h"

// Sets default values
ARoomDoor::ARoomDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> Steel_Barred_0(TEXT("StaticMesh'/Game/Template/Steel_Barred__1fbx.Steel_Barred__1fbx'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Steel_Barred_1(TEXT("StaticMesh'/Game/Template/Steel_Barred.Steel_Barred'"));

	Walls_0 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh0"));
	Walls_1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh1"));
	Walls_2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh2"));

	Walls_0->AttachTo(RootComponent);
	Walls_0->SetStaticMesh(Steel_Barred_0.Object);
	Walls_0->RelativeScale3D = FVector(1, 1, 2);
	Walls_0->RelativeLocation = FVector(0, 0, -200);

	Walls_1->AttachTo(RootComponent);
	Walls_1->SetStaticMesh(Steel_Barred_1.Object);
	Walls_1->RelativeLocation = FVector(0, 0, -200);

	Walls_2->AttachTo(RootComponent);
	Walls_2->SetStaticMesh(Steel_Barred_1.Object);
	Walls_2->RelativeLocation = FVector(0, 0, -190);
}

void ARoomDoor::BeginPlay()
{
	Super::BeginPlay();

}
