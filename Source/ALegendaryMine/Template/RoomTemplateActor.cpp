// Fill out your copyright notice in the Description page of Project Settings.


#include "RoomTemplateActor.h"
#include "Engine.h"

// Sets default values
ARoomTemplateActor::ARoomTemplateActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Walls = NewObject<UInstancedStaticMeshComponent>(this, TEXT("Walls"));
	Walls->AttachTo(RootComponent);
}

void ARoomTemplateActor::SetAsset(UMyCustomAsset* _MyCustomAsset)
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

		ActorArr.Add(
			GetWorld()->SpawnActor<AActor>(
				StaticMesh1->GeneratedClass,
				MyCustomAsset->ActorArr[i].MeshTransform));

		ActorArr[ActorArr.Num() - 1]->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void ARoomTemplateActor::CreateRoad(FVector _RoadLo, FVector _Lo)
{
	bool bCheck = true;

	//검은벽 제거 
	for (int ii = 0; ii < MyCustomAsset->ActorData.Num(); ii++)
		for (int i = 0; i < MyCustomAsset->ActorData[ii].ActorData.Num(); i++)
		{
			FTransform InstanceTrans = FTransform::Identity;
			InstanceActor[ii]->GetInstanceTransform(i, InstanceTrans, true);

			if (InstanceTrans.GetLocation().X == _Lo.X && InstanceTrans.GetLocation().Y == _Lo.Y)
			{
				for (int k = 0; k < MyCustomAsset->ActorData[0].ActorData.Num(); k++)
				{
					FTransform InstanceTrans = FTransform::Identity;
					InstanceActor[0]->GetInstanceTransform(k, InstanceTrans, true);

					if (InstanceTrans.GetLocation().X == _Lo.X &&
						InstanceTrans.GetLocation().Y == _Lo.Y &&
						InstanceTrans.GetLocation().Z >= 0)
					{
						InstanceActor[0]->UpdateInstanceTransform(k, FTransform(FRotator(), FVector(0, 0, -300), FVector(0, 0, 0)), true, true, true);

						/*타일을 붙이는 작업*/
						//삭제된 검은벽 주변에 있는 검은벽을 기준으로 +로 검은벽을 검색하고*
						//검은벽이 있으면 타일을 부착*
						//Lo와 같은 위치에 있었으면 패스*

						if (bFindBlackWall)
						{
							for (int j = 0; j < MyCustomAsset->ActorData[0].ActorData.Num(); j++)
							{
								FTransform InstanceTrans = FTransform::Identity;
								InstanceActor[0]->GetInstanceTransform(j, InstanceTrans, true);

								if ((
									((InstanceTrans.GetLocation().Y == _Lo.Y) && (InstanceTrans.GetLocation().X == _Lo.X + 100 || InstanceTrans.GetLocation().X == _Lo.X - 100)) ||
									((InstanceTrans.GetLocation().X == _Lo.X) && (InstanceTrans.GetLocation().Y == _Lo.Y + 100 || InstanceTrans.GetLocation().Y == _Lo.Y - 100))) &&
									InstanceTrans.GetLocation().Z >= 0)
								{
									FRotator Rot = (InstanceTrans.GetLocation() - _Lo).Rotation();
									Rot.Pitch = 0;
									Rot.Yaw += 90;

									Walls->AddInstanceWorldSpace(FTransform(Rot, FVector(InstanceTrans.GetLocation().X, InstanceTrans.GetLocation().Y, 0), FVector(1, 1, 1)));
									Walls->AddInstanceWorldSpace(FTransform(Rot, FVector(InstanceTrans.GetLocation().X, InstanceTrans.GetLocation().Y, 100), FVector(1, 1, 1)));

									UE_LOG(LogTemp, Log, TEXT("Add Room Walls"));
								}
							}
						}

						bCheck = false;
						bFindBlackWall = true;
					}
				}
			}
		}

	if (bCheck && bFindBlackWall) //문 앞 한칸까지만의 오브젝트를 제거하기 위한 장치
		iCount--;

	//문 앞에 있는 오브젝트 제거
	for (int ii = 0; ii < MyCustomAsset->ActorData.Num(); ii++)
		for (int i = 0; i < MyCustomAsset->ActorData[ii].ActorData.Num(); i++)
		{
			FTransform InstanceTrans = FTransform::Identity;
			InstanceActor[ii]->GetInstanceTransform(i, InstanceTrans, true);

			if (InstanceTrans.GetLocation().X == _Lo.X && InstanceTrans.GetLocation().Y == _Lo.Y)
			{
				for (int j = 1; j < MyCustomAsset->ActorData.Num(); j++)
					for (int k = 0; k < MyCustomAsset->ActorData[j].ActorData.Num(); k++)
					{
						FTransform InstanceTrans = FTransform::Identity;
						InstanceActor[j]->GetInstanceTransform(k, InstanceTrans, true);

						if (InstanceTrans.GetLocation().X == _Lo.X &&
							InstanceTrans.GetLocation().Y == _Lo.Y &&
							InstanceTrans.GetLocation().Z >= 0)
						{
							InstanceActor[j]->UpdateInstanceTransform(k, FTransform(FRotator(), FVector(0, 0, -300), FVector(0, 0, 0)), true, true, true);
							bCheck = false;
						}
					}

				for (int j = 0; j < Walls->GetNumRenderInstances(); j++)
				{
					FTransform InstanceTrans = FTransform::Identity;
					Walls->GetInstanceTransform(j, InstanceTrans, true);

					if (InstanceTrans.GetLocation().X == _Lo.X &&
						InstanceTrans.GetLocation().Y == _Lo.Y &&
						InstanceTrans.GetLocation().Z >= 0)
					{
						Walls->UpdateInstanceTransform(j, FTransform(FRotator(), FVector(0, 0, -300), FVector(0, 0, 0)), true, true, true);
						bCheck = false;
					}
				}
			}
		}

	for (int i = 0; i < ActorArr.Num(); i++) //문 앞에 있는 물체 제거
	{
		FTransform InstanceTrans = ActorArr[i]->GetActorTransform();

		if (InstanceTrans.GetLocation().X == _Lo.X &&
			InstanceTrans.GetLocation().Y == _Lo.Y &&
			InstanceTrans.GetLocation().Z >= 0)
		{
			ActorArr[i]->Destroy();
			bCheck = false;
		}
	}

	if (bCheck || iCount == 0)
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

void ARoomTemplateActor::DestroyRoom()
{
	for (int i = 0; i < ActorArr.Num(); i++)
	{
		ActorArr[i]->Destroy();
	}
}

void ARoomTemplateActor::SetRoadMeshData(FRoadMeshData _MeshData)
{
	Walls->SetStaticMesh(_MeshData.Walls);
}

// Called when the game starts or when spawned
void ARoomTemplateActor::BeginPlay()
{
	Super::BeginPlay();

}
