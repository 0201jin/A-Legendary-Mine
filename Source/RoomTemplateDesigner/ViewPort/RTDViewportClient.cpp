// Fill out your copyright notice in the Description page of Project Settings.


#include "RTDViewportClient.h"
#include "SRTDViewport.h"
#include "CustomAssetEditor.h"
#include "AdvancedPreviewScene.h"
#include "MyCustomAsset.h"
#include "EngineUtils.h"

#include "EditorStyleSet.h"
#include "Editor/EditorPerProjectUserSettings.h"
#include "AssetViewerSettings.h"

FRTDViewportClient::FRTDViewportClient(TWeakPtr<class FCustomAssetEditor> ParentIGCEditor, const TSharedRef<class FPreviewScene>& AdvPreviewScene, const TSharedRef<class SRTDViewport>& IGCViewport, UMyCustomAsset * ObjectToEdit)
	: FEditorViewportClient(nullptr, &AdvPreviewScene.Get(), StaticCastSharedRef<SEditorViewport>(IGCViewport))
	, IGCEditorPtr(ParentIGCEditor)
	, IGCEditorViewportPtr(IGCViewport)
	, IGCObject(ObjectToEdit)
{
	bW = false;
	bS = false;
	bA = false;
	bD = false;
	bRB = false;

	SetViewMode(VMI_Lit);

	AdvancedPreviewScene = static_cast<FPreviewScene*>(PreviewScene);

	SetViewLocation(FVector(0.0f, 0.0f, 500.0f));
	SetViewRotation(FRotator(-90.0f, 0.0f, 0.0f));
	SetViewLocationForOrbiting(FVector::ZeroVector, 500.0f);

	UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Template/Box.Box'"), NULL, LOAD_None, NULL);

	MeshActor = NewObject<UStaticMeshComponent>(GetTransientPackage(), NAME_None, RF_Transient);
	MeshActor->SetStaticMesh(StaticMesh);
	MeshActor->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTransform Transform = FTransform::Identity;
	Transform.SetLocation(FVector(0, 0, 0));
	PreviewScene->AddComponent(MeshActor, Transform);

	actor.Empty();

	if (IGCObject->MeshDataArr.Num() == 0)
	{
		FMeshData data;
		data.MeshData = "StaticMesh'/Game/Template/BlackWall.BlackWall'"; //검은색 기둥 넣기

		IGCObject->MeshDataArr.Add(data);
	}

	for (int i = 0; IGCObject->ActorData.Num() > i; i++)
	{
		UStaticMesh* ISMStaticMesh = LoadObject<UStaticMesh>(NULL, *IGCObject->ActorData[i].Meshdata, NULL, LOAD_None, NULL);

		actor.Add(NewObject<UInstancedStaticMeshComponent>(GetTransientPackage(), NAME_None, RF_Transient));
		actor[i]->SetStaticMesh(ISMStaticMesh);
		actor[i]->PerInstanceSMData = IGCObject->ActorData[i].ActorData;

		Transform.SetRotation(FQuat(0, 0, 0, 0));

		PreviewScene->AddComponent(actor[i], Transform);
	}

	for (int i = 0; IGCObject->ActorArr.Num() > i; i++)
	{
		UBlueprint* StaticMesh1 = LoadObject<UBlueprint>(NULL, *IGCObject->ActorArr[i].MeshData, NULL, LOAD_None, NULL);
		
		Actors.Add(PreviewScene->GetWorld()->SpawnActor<AActor>(StaticMesh1->GeneratedClass, IGCObject->ActorArr[i].MeshTransform));
	}
}

FRTDViewportClient::~FRTDViewportClient()
{
}

void FRTDViewportClient::CreateWalls(int _x, int _y)
{
	if (IGCObject->MeshDataArr[0].Index == -1)
	{
		ActorIndex = IGCObject->ActorData.Num();

		UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(NULL, *IGCObject->MeshDataArr[0].MeshData, NULL, LOAD_None, NULL);
		actor.Add(NewObject<UInstancedStaticMeshComponent>(GetTransientPackage(), NAME_None, RF_Transient));
		actor[ActorIndex]->SetStaticMesh(StaticMesh);

		IGCObject->ActorData.Add(FISMData());
		IGCObject->ActorData[ActorIndex].ActorData = actor[ActorIndex]->PerInstanceSMData;
		IGCObject->ActorData[ActorIndex].Meshdata = IGCObject->MeshDataArr[0].MeshData;

		IGCObject->MeshDataArr[0].Index = ActorIndex;
	}

	for (int x = 0; x < _x; x++)
	{
		FTransform Transform = FTransform::Identity;
		Transform.SetLocation(FVector(x * 100, 0, 0));
		Transform.SetRotation(MeshActor->GetRelativeRotation().Quaternion());
		actor[IGCObject->MeshDataArr[0].Index]->AddInstanceWorldSpace(Transform);

		Transform.SetLocation(FVector(x * 100, (_y - 1) * 100, 0));
		Transform.SetRotation(MeshActor->GetRelativeRotation().Quaternion());
		actor[IGCObject->MeshDataArr[0].Index]->AddInstanceWorldSpace(Transform);
	}

	for (int y = 1; y < (_y - 1); y++)
	{
		FTransform Transform = FTransform::Identity;
		Transform.SetLocation(FVector(0, y * 100, 0));
		Transform.SetRotation(MeshActor->GetRelativeRotation().Quaternion());
		actor[IGCObject->MeshDataArr[0].Index]->AddInstanceWorldSpace(Transform);

		Transform.SetLocation(FVector((_x - 1) * 100, y * 100, 0));
		Transform.SetRotation(MeshActor->GetRelativeRotation().Quaternion());
		actor[IGCObject->MeshDataArr[0].Index]->AddInstanceWorldSpace(Transform);
	}

	FTransform Transform = FTransform::Identity;
	Transform.SetRotation(FQuat(0, 0, 0, 0));
	PreviewScene->AddComponent(actor[IGCObject->MeshDataArr[0].Index], Transform);
	IGCObject->ActorData[IGCObject->MeshDataArr[0].Index].ActorData = actor[IGCObject->MeshDataArr[0].Index]->PerInstanceSMData;

	IGCObject->SX = _x;
	IGCObject->SY = _y;

	UE_LOG(LogTemp, Log, TEXT("%d %d"), _x, _y);
}

void FRTDViewportClient::Tick(float DeltaSeconds)
{
	if (bW)
		MoveViewportCamera(GetViewRotation().Vector().ToOrientationQuat().GetForwardVector() * 10, FRotator(0, 0, 0));
	if (bS)
		MoveViewportCamera(GetViewRotation().Vector().ToOrientationQuat().GetForwardVector() * -10, FRotator(0, 0, 0));
	if (bA)
		MoveViewportCamera(GetViewRotation().Vector().ToOrientationQuat().GetRightVector() * -10, FRotator(0, 0, 0));
	if (bD)
		MoveViewportCamera(GetViewRotation().Vector().ToOrientationQuat().GetRightVector() * 10, FRotator(0, 0, 0));

	Viewport->Draw();

	FEditorViewportClient::Tick(DeltaSeconds);
}

void FRTDViewportClient::Draw(const FSceneView * View, FPrimitiveDrawInterface * PDI)
{
	FEditorViewportClient::Draw(View, PDI);
}

void FRTDViewportClient::MouseMove(FViewport * Viewport, int32 x, int32 y)
{
	FViewportCursorLocation CursorLo = GetCursorWorldLocationFromMousePos();
	FVector Direction = CursorLo.GetDirection();
	FVector Location = CursorLo.GetOrigin();

	FRotator DiRo = ((Direction + Location) - Location).Rotation();
	double C = ((-1 * DiRo.Pitch) / 180) * PI;
	float BC = (Location.Z - MeshActor->GetRelativeLocation().Z) / sin(C);

	FVector WorldLocation = (Direction * BC) + Location;
	WorldLocation.X = int(WorldLocation.X / 100) * 100;
	WorldLocation.Y = int(WorldLocation.Y / 100) * 100;
	WorldLocation.Z = MeshActor->GetRelativeLocation().Z;//int(WorldLocation.Z / 100) * 100;

	/*FHitResult result;
	GetWorld()->LineTraceSingleByChannel(result, Location, WorldLocation, ECollisionChannel::ECC_WorldDynamic);

	if (result.GetComponent() != nullptr)
	{
		UInstancedStaticMeshComponent* ISM = Cast<UInstancedStaticMeshComponent>(result.GetComponent());

		FTransform Transform = FTransform::Identity;
		ISM->GetInstanceTransform(result.Item, Transform);

		WorldLocation.Z = Transform.GetLocation().Z + 100;
	}*/

	MeshActor->SetWorldLocation(WorldLocation);

	Invalidate();
}

bool FRTDViewportClient::InputKey(const FInputKeyEventArgs & EventArgs)
{
	if (EventArgs.Event == EInputEvent::IE_Released)
	{
		if (EventArgs.Key == EKeys::W)
		{
			bW = false;
		}

		else if (EventArgs.Key == EKeys::S)
		{
			bS = false;
		}

		else if (EventArgs.Key == EKeys::A)
		{
			bA = false;
		}

		else if (EventArgs.Key == EKeys::D)
		{
			bD = false;
		}

		else if (EventArgs.Key == EKeys::RightMouseButton)
		{
			bRB = false;
		}

		else if (EventArgs.Key == EKeys::LeftShift)
		{
			bShift = false;
		}
	}

	else if (EventArgs.Event == EInputEvent::IE_Pressed)
	{
		if (EventArgs.Key == EKeys::W)
		{
			bW = true;
		}

		else if (EventArgs.Key == EKeys::S)
		{
			bS = true;
		}

		else if (EventArgs.Key == EKeys::A)
		{
			bA = true;
		}

		else if (EventArgs.Key == EKeys::D)
		{
			bD = true;
		}

		else if (EventArgs.Key == EKeys::R)
		{
			MeshActor->AddWorldRotation(FRotator(0, 45, 0));
		}

		else if (EventArgs.Key == EKeys::RightMouseButton)
		{
			bRB = true;
		}

		else if (EventArgs.Key == EKeys::LeftShift)
		{
			bShift = true;
		}

		else if (EventArgs.Key == EKeys::One)
		{
			MeshActor->AddWorldOffset(FVector(0, 0, 100));
		}
		
		else if (EventArgs.Key == EKeys::Two)
		{
			MeshActor->AddWorldOffset(FVector(0, 0, -100));
		}

		else if (EventArgs.Key == EKeys::LeftMouseButton)
		{
			FVector WorldLocation = MeshActor->GetRelativeLocation();

			if (!IGCObject->bSelectActor)
			{
				if (IGCObject->MeshDataArr.Num() > 0)
				{
					if (!bShift)
					{
						if (IGCObject->MeshDataArr[IGCObject->MeshDataIndex].Index == -1)
						{
							UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(NULL, *IGCObject->MeshDataArr[IGCObject->MeshDataIndex].MeshData, NULL, LOAD_None, NULL);
							ActorIndex = IGCObject->ActorData.Num();

							actor.Add(NewObject<UInstancedStaticMeshComponent>(GetTransientPackage(), NAME_None, RF_Transient));
							actor[ActorIndex]->SetStaticMesh(StaticMesh);

							IGCObject->ActorData.Add(FISMData());
							IGCObject->ActorData[ActorIndex].ActorData = actor[ActorIndex]->PerInstanceSMData;
							IGCObject->ActorData[ActorIndex].Meshdata = IGCObject->MeshDataArr[IGCObject->MeshDataIndex].MeshData;

							IGCObject->MeshDataArr[IGCObject->MeshDataIndex].Index = ActorIndex;

							FTransform Transform = FTransform::Identity;
							Transform.SetLocation(FVector(WorldLocation.X, WorldLocation.Y, WorldLocation.Z));
							Transform.SetRotation(MeshActor->GetRelativeRotation().Quaternion());
							actor[ActorIndex]->AddInstanceWorldSpace(Transform);

							Transform.SetRotation(FQuat(0, 0, 0, 0));
							PreviewScene->AddComponent(actor[ActorIndex], Transform);
						}
						else
						{
							ActorIndex = IGCObject->MeshDataArr[IGCObject->MeshDataIndex].Index;

							FTransform Transform = FTransform::Identity;
							Transform.SetLocation(FVector(WorldLocation.X, WorldLocation.Y, WorldLocation.Z));
							Transform.SetRotation(MeshActor->GetRelativeRotation().Quaternion());
							actor[ActorIndex]->AddInstanceWorldSpace(Transform);

							IGCObject->ActorData[ActorIndex].ActorData = actor[ActorIndex]->PerInstanceSMData;
						}
					}
					else
					{
						if (iClickCount < 2)
						{
							if (iClickCount == 0)
								ClickV1 = WorldLocation;
							else
								ClickV2 = WorldLocation;

							iClickCount++;
						}

						if (iClickCount >= 2)
						{
							int LX = ClickV1.X;
							int LY = ClickV1.Y;
							int LZ = ClickV1.Z;

							int SX = ClickV2.X;
							int SY = ClickV2.Y;
							int SZ = ClickV2.Z;

							if (ClickV1.X < ClickV2.X)
							{
								LX = ClickV2.X;
								SX = ClickV1.X;
							}

							if (ClickV1.Y < ClickV2.Y)
							{
								LY = ClickV2.Y;
								SY = ClickV1.Y;
							}

							if (ClickV1.Z < ClickV2.Z)
							{
								LZ = ClickV2.Z;
								SZ = ClickV1.Z;
							}

							if (IGCObject->MeshDataArr[IGCObject->MeshDataIndex].Index == -1)
							{
								UStaticMesh* StaticMesh = LoadObject<UStaticMesh>(NULL, *IGCObject->MeshDataArr[IGCObject->MeshDataIndex].MeshData, NULL, LOAD_None, NULL);

								ActorIndex = IGCObject->ActorData.Num();

								actor.Add(NewObject<UInstancedStaticMeshComponent>(GetTransientPackage(), NAME_None, RF_Transient));
								actor[ActorIndex]->SetStaticMesh(StaticMesh);

								IGCObject->ActorData.Add(FISMData());
								IGCObject->ActorData[ActorIndex].ActorData = actor[ActorIndex]->PerInstanceSMData;
								IGCObject->ActorData[ActorIndex].Meshdata = IGCObject->MeshDataArr[IGCObject->MeshDataIndex].MeshData;

								IGCObject->MeshDataArr[IGCObject->MeshDataIndex].Index = ActorIndex;

								FTransform Transform = FTransform::Identity;
								PreviewScene->AddComponent(actor[ActorIndex], Transform);

								UE_LOG(LogTemp, Log, TEXT("Obj Spawn"));
							}

							ActorIndex = IGCObject->MeshDataArr[IGCObject->MeshDataIndex].Index;

							for (int x = SX; x <= LX;)
							{
								for (int y = SY; y <= LY;)
								{
									for (int z = SZ; z <= LZ;)
									{
										FTransform Transform = FTransform::Identity;
										Transform.SetLocation(FVector(x, y, z));
										Transform.SetRotation(MeshActor->GetRelativeRotation().Quaternion());
										actor[ActorIndex]->AddInstanceWorldSpace(Transform);

										UE_LOG(LogTemp, Log, TEXT("%d %d %d"), x, y, z);

										z += 100;
									}
									y += 100;
								}
								x += 100;
							}

							UE_LOG(LogTemp, Log, TEXT("END"));

							IGCObject->ActorData[ActorIndex].ActorData = actor[ActorIndex]->PerInstanceSMData;

							bShift = false;
							iClickCount = 0;
						}
					}
				}
			}
			else
			{
				UBlueprint* StaticMesh1 = LoadObject<UBlueprint>(NULL, *IGCObject->ActorDataArr[IGCObject->ActorDataIndex].MeshData, NULL, LOAD_None, NULL);

				if (StaticMesh1)
				{
					FTransform Transform = FTransform::Identity;
					Transform.SetLocation(FVector(WorldLocation.X, WorldLocation.Y, WorldLocation.Z));
					Transform.SetRotation(MeshActor->GetRelativeRotation().Quaternion());

					Actors.Add(PreviewScene->GetWorld()->SpawnActor<AActor>(
						StaticMesh1->GeneratedClass, Transform));

					ActorIndex = Actors.Num() - 1;

					Actors[ActorIndex]->ApplyWorldOffset(Transform.GetLocation(), true);

					FAData Data;
					Data.MeshData = IGCObject->ActorDataArr[IGCObject->ActorDataIndex].MeshData;
					Data.MeshTransform = Transform;

					IGCObject->ActorArr.Add(Data);

					UE_LOG(LogTemp, Log, TEXT("Actor %s"), *IGCObject->ActorDataArr[IGCObject->ActorDataIndex].MeshData);
				}
			}
		}

		else if (EventArgs.Key == EKeys::Q)
		{
			FViewportCursorLocation CursorLo = GetCursorWorldLocationFromMousePos();
			FVector Direction = CursorLo.GetDirection();
			FVector Location = CursorLo.GetOrigin();

			FRotator DiRo = ((Direction + Location) - Location).Rotation();
			double C = ((-1 * DiRo.Pitch) / 180) * PI;
			float BC = (Location.Z - MeshActor->GetRelativeLocation().Z) / sin(C);

			FVector WorldLocation = (Direction * BC) + Location;
			WorldLocation.X = int(WorldLocation.X / 100) * 100;
			WorldLocation.Y = int(WorldLocation.Y / 100) * 100;
			WorldLocation.Z = MeshActor->GetRelativeLocation().Z; //int(WorldLocation.Z / 100) * 100;

			FHitResult result;
			GetWorld()->LineTraceSingleByChannel(result, Location, WorldLocation, ECollisionChannel::ECC_WorldDynamic);

			if (result.GetComponent() != nullptr)
			{
				int Index = -1;

				if (actor.Num() > 0)
				{
					for (int i = 0; i < actor.Num(); i++)
					{
						if (Cast<UInstancedStaticMeshComponent>(result.GetComponent()) == actor[i])
						{
							Index = i;
							break;
						}
					}

					if (Index != -1)
					{
						actor[Index]->RemoveInstance(result.Item);
						IGCObject->ActorData[Index].ActorData = actor[Index]->PerInstanceSMData;
					}
				}

				if (Actors.Num() > 0)
				{
					Index = -1;

					for (int i = 0; i < Actors.Num(); i++)
					{
						if (Cast<AActor>(result.GetActor()) == Actors[i])
						{
							Index = i;
							break;
						}
					}

					if (Index != -1)
					{
						Actors[Index]->Destroy();
						Actors.RemoveAt(Index);
						IGCObject->ActorArr.RemoveAt(Index);
					}
				}

				Viewport->Draw();
			}
		}
	}

	return true;
}

bool FRTDViewportClient::InputAxis(FViewport * Viewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	//축회전 하는 코드

	if (bRB)
	{
		if (Key == EKeys::MouseX)
			MoveViewportCamera(FVector(0, 0, 0), FRotator(0, Delta / 2, 0));

		if (Key == EKeys::MouseY)
			MoveViewportCamera(FVector(0, 0, 0), FRotator(Delta / 2, 0, 0));
	}

	return true;
}