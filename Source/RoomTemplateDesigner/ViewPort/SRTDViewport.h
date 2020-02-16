// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "SlateFwd.h"
#include "UObject/GCObject.h"
#include "SEditorViewport.h"
#include "SCommonEditorViewportToolbarBase.h"
#include "MyCustomAsset.h"

class FCustomAssetEditor;
class UMyCustomAsset;

class SRTDViewport : public SEditorViewport, public FGCObject
{
public:
	SLATE_BEGIN_ARGS(SRTDViewport) {}
	SLATE_ARGUMENT(TWeakPtr<FCustomAssetEditor>, ParentIGCEditor)
		SLATE_ARGUMENT(UMyCustomAsset*, ObjectToEdit)
		SLATE_END_ARGS()

		void Construct(const FArguments& InArgs);
	SRTDViewport();
	~SRTDViewport();

	void CreateWalls(int _x, int _y);

	// FGCObject �������̽�. U������Ʈ�� �ƴ϶� �������÷��� ȸ�� ����.
	virtual void AddReferencedObjects(FReferenceCollector& Collector) override;
	TSharedRef<class FPreviewScene> GetPreviewScene();

protected:
	// �Ʒ� �Լ��� ������ ����Ʈ Ŭ���̾�Ʈ�� �������־�� �Ѵ�.
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;

private:
	// �θ� ������, ������ ��, ����Ʈ Ŭ���̾�Ʈ, �Ѱܹ��� IGC ��ü. 
	TWeakPtr<FCustomAssetEditor> IGCEditorPtr;
	TSharedPtr<class FPreviewScene> PreviewScene;
	TSharedPtr<class FRTDViewportClient> IGCViewportClient;
	UMyCustomAsset* IGCObject;

	// �������̿� ����� ��Ƽ�� �ڽ� ����.
	TSharedPtr<SVerticalBox> OverlayTextVerticalBox;
};
