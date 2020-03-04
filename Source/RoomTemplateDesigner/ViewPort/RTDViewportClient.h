// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorViewportClient.h"
#include "MyCustomAsset.h"

/**
 *
 */

class UMyCustomAsset;

class FRTDViewportClient : public FEditorViewportClient, public TSharedFromThis<FRTDViewportClient>
{
public:
	FRTDViewportClient(TWeakPtr<class FCustomAssetEditor> ParentIGCEditor, const TSharedRef<class FPreviewScene>& AdvPreviewScene, const TSharedRef<class SRTDViewport>& IGCViewport, UMyCustomAsset* ObjectToEdit);
	~FRTDViewportClient();

	void CreateWalls(int _x, int _y);

	// 에디터에서 사용하는 입력
	virtual void Tick(float DeltaSeconds) override;
	virtual void Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	virtual void MouseMove(FViewport * Viewport, int32 x, int32 y);
	virtual bool InputKey(const FInputKeyEventArgs & EventArgs);
	virtual bool InputAxis(FViewport * Viewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad);

private:
	TWeakPtr<class FCustomAssetEditor> IGCEditorPtr;
	TWeakPtr<class SRTDViewport> IGCEditorViewportPtr;
	UMyCustomAsset* IGCObject;
	class FAdvancedPreviewScene* AdvancedPreviewScene;

	class UStaticMeshComponent * MeshActor;

	TArray<UInstancedStaticMeshComponent*> actor;
	TArray<AActor*> Actors;

	bool bW, bA, bS, bD, bRB, bShift;

	int iClickCount = 0;
	FVector ClickV1, ClickV2;
	int ActorIndex = 0;
};
