// Fill out your copyright notice in the Description page of Project Settings.


#include "SRTDViewport.h"
#include "RTDViewportClient.h"
#include "AdvancedPreviewScene.h"

#define LOCTEXT_NAMESPACE "RTDViewport"

SRTDViewport::SRTDViewport()
	: PreviewScene(MakeShareable(new FPreviewScene(FPreviewScene::ConstructionValues())))
{
}

SRTDViewport::~SRTDViewport()
{
	if (IGCViewportClient.IsValid())
	{
		IGCViewportClient->Viewport = NULL;
	}
}

void SRTDViewport::CreateWalls(int _x, int _y)
{
	IGCViewportClient->CreateWalls(_x, _y);
}

void SRTDViewport::AddReferencedObjects(FReferenceCollector & Collector)
{
	Collector.AddReferencedObject(IGCObject);
}

TSharedRef<class FPreviewScene> SRTDViewport::GetPreviewScene()
{
	return PreviewScene.ToSharedRef();
}

TSharedRef<FEditorViewportClient> SRTDViewport::MakeEditorViewportClient()
{
	IGCViewportClient = MakeShareable(new FRTDViewportClient(IGCEditorPtr, PreviewScene.ToSharedRef(), SharedThis(this), IGCObject));

	return IGCViewportClient.ToSharedRef();
}

void SRTDViewport::Construct(const FArguments& InArgs)
{
	IGCEditorPtr = InArgs._ParentIGCEditor;
	IGCObject = InArgs._ObjectToEdit;

	SEditorViewport::Construct(SEditorViewport::FArguments());

	ViewportOverlay->AddSlot()
		.VAlign(VAlign_Top)
		.HAlign(HAlign_Left)
		.Padding(FMargin(10.0f, 10.0f, 10.0f, 10.0f))
		[
			SAssignNew(OverlayTextVerticalBox, SVerticalBox)
		];

	OverlayTextVerticalBox->ClearChildren();
	OverlayTextVerticalBox->AddSlot()
		[
			SNew(STextBlock)
			.Text(LOCTEXT("IGCWelcomeText", "Welcome To IGC 2018"))
		.TextStyle(FEditorStyle::Get(), TEXT("TextBlock.ShadowedText"))
		.ColorAndOpacity(FLinearColor::Red)
		];
}

#undef LOCTEXT_NAMESPACE