// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomTemplateActor.h"
#include "Engine.h"

// Sets default values
ARoomTemplateActor::ARoomTemplateActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ARoomTemplateActor::SetAsset(UMyCustomAsset * _MyCustomAsset)
{
	InstanceActor.Empty();

	MyCustomAsset = _MyCustomAsset;

	for (int i = 0; i < MyCustomAsset->ActorData.Num(); i++)
	{
		UStaticMesh* ISMStaticMesh = LoadObject<UStaticMesh>(NULL, *MyCustomAsset->ActorData[i].Meshdata, NULL, LOAD_None, NULL);
		UInstancedStaticMeshComponent* ISM = NewObject<UInstancedStaticMeshComponent>(this, TEXT("Template"));
		InstanceActor.Add(ISM);
		InstanceActor[i]->RegisterComponent();
		InstanceActor[i]->AttachTo(RootComponent);

		InstanceActor[i]->SetStaticMesh(ISMStaticMesh);
		InstanceActor[i]->PerInstanceSMData = MyCustomAsset->ActorData[i].ActorData;

		InstanceActor[i]->UpdateInstanceTransform(0, FTransform::Identity, true, true);

		InstanceActor[i]->MarkRenderStateDirty();
	}
}

// Called when the game starts or when spawned
void ARoomTemplateActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARoomTemplateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

