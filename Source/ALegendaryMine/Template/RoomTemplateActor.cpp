// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomTemplateActor.h"
#include "Engine.h"

// Sets default values
ARoomTemplateActor::ARoomTemplateActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ARoomTemplateActor::SetAsset(UMyCustomAsset * _MyCustomAsset)
{
	InstanceActor.Empty();

	MyCustomAsset = _MyCustomAsset;

	for (int i = 0; i < MyCustomAsset->ActorData.Num(); i++)
	{
		FString ISMName = FString::FromInt(i);
		UStaticMesh* ISMStaticMesh = LoadObject<UStaticMesh>(NULL, *MyCustomAsset->ActorData[i].Meshdata, NULL, LOAD_None, NULL);
		UInstancedStaticMeshComponent* ISM = NewObject<UInstancedStaticMeshComponent>(this, *ISMName);
		InstanceActor.Add(ISM);
		InstanceActor[i]->OnComponentCreated();
		InstanceActor[i]->RegisterComponent();

		if (InstanceActor[i]->bWantsInitializeComponent)
			InstanceActor[i]->InitializeComponent();

		InstanceActor[i]->AttachTo(RootComponent);

		InstanceActor[i]->SetStaticMesh(ISMStaticMesh);

		for (int j = 0; j < MyCustomAsset->ActorData[i].ActorData.Num(); j++)
		{
			InstanceActor[i]->AddInstanceWorldSpace(FTransform(MyCustomAsset->ActorData[i].ActorData[j].Transform));
		}
	}

	for (int i = 0; i < MyCustomAsset->ActorArr.Num(); i++)
	{
		UBlueprint* StaticMesh1 = LoadObject<UBlueprint>(NULL, *MyCustomAsset->ActorArr[i].MeshData, NULL, LOAD_None, NULL);

		GetWorld()->SpawnActor<AActor>(StaticMesh1->GeneratedClass, MyCustomAsset->ActorArr[i].MeshTransform)->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void ARoomTemplateActor::CreateRoad(FVector _RoadLo, FVector _Lo)
{
	bool bCheck = true;
	
	for (int i = 0; i < MyCustomAsset->ActorData[0].ActorData.Num(); i++)
	{
		FTransform InstanceTrans = FTransform::Identity;
		InstanceActor[0]->GetInstanceTransform(i, InstanceTrans, true);

		if (InstanceTrans.GetLocation().X == _Lo.X && InstanceTrans.GetLocation().Y == _Lo.Y)
		{
			for (int j = 0; j < MyCustomAsset->ActorData.Num(); j++)
				for (int k = 0; k < MyCustomAsset->ActorData[j].ActorData.Num(); k++)
				{
					FTransform InstanceTrans = FTransform::Identity;
					InstanceActor[j]->GetInstanceTransform(k, InstanceTrans, true);

					if (InstanceTrans.GetLocation().X == _Lo.X &&
						InstanceTrans.GetLocation().Y == _Lo.Y)
					{
						//InstanceActor[j]->RemoveInstance(k);
						InstanceActor[j]->UpdateInstanceTransform(k, FTransform(FRotator(), FVector(0, 0, -300), FVector(0, 0, 0)), true, true, true);
						bCheck = false;
					}
				}
		}
	}

	if (bCheck)
		return;

	if (_RoadLo.X == _Lo.X)
	{
		CreateRoad(_Lo, FVector(_Lo.X, _Lo.Y + (_Lo.Y - _RoadLo.Y), 0));
	}
	else if (_RoadLo.Y == _Lo.Y)
	{
		CreateRoad(_Lo, FVector(_Lo.X + (_Lo.X - _RoadLo.X), _Lo.Y, 0));
	}
}

// Called when the game starts or when spawned
void ARoomTemplateActor::BeginPlay()
{
	Super::BeginPlay();

}
