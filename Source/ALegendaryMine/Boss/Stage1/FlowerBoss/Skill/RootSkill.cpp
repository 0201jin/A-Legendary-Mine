// Fill out your copyright notice in the Description page of Project Settings.


#include "RootSkill.h"
#include "Engine.h"

// Sets default values
ARootSkill::ARootSkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static UStaticMesh* RootMesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Boss/1Stage/Flower/Skill/Cone.Cone'"));
	static UMaterial* RootMat = LoadObject<UMaterial>(NULL, TEXT("Material'/Game/Boss/1Stage/Flower/Skill/DefaultMaterial.DefaultMaterial'"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Mesh = NewObject<UInstancedStaticMeshComponent>(this, TEXT("RootMesh"));
	Mesh->SetStaticMesh(RootMesh);
	Mesh->AttachTo(RootComponent);
	Mesh->SetRelativeLocation(FVector(0, 0, 0));
}

// Called when the game starts or when spawned
void ARootSkill::BeginPlay()
{
	Super::BeginPlay();
	
	int r = (rand() % 4) + 4;

	for (int i = 0; i < r; i++)
	{
		FVector Location = GetActorLocation();
		Location.X += (rand() % 100) - 50;
		Location.Y += (rand() % 100) - 50;
		Location.Z = 0;

		FVector Scale = FVector(0.1, 0.1, 1);
		int rs = ((rand() % 5)) * 0.1;
		Scale.X += rs;
		Scale.Y += rs;
		Scale.Z += ((rand() % 20)) * 0.1;

		FRotator Rotation = FRotator(0, 0, 0);
		Rotation.Yaw = (rand() % 50) - 25;
		Rotation.Pitch = (rand() % 50) - 25;

		Mesh->AddInstanceWorldSpace(FTransform(Rotation, Location, Scale));
	}
}

// Called every frame
void ARootSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

