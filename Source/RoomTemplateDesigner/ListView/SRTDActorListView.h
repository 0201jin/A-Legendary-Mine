// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "MyCustomAsset.h"
#include "WorkflowOrientedApp/SContentReference.h"

class UMyCustomAsset;
class FCustomAssetEditor;

class SRTDActorListView : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRTDActorListView) {}
	SLATE_ARGUMENT(UMyCustomAsset*, ObjectToEdit)
		SLATE_END_ARGS()

	void Construct(const FArguments& Args);
	FReply ButtonPressed();
	void SetReference(UObject* Object);
	UObject* GetFreference() const;
	void ListItemClick(TSharedPtr<FActorData> SelectItem);

	/* Adds a new textbox with the string to the list */
	TSharedRef<class ITableRow> OnGenerateRowForList(TSharedPtr<FActorData> Item, const TSharedRef<class STableViewBase>& OwnerTable);

	/* The list of strings */
	TArray<TSharedPtr<FActorData>> Items;

	UMyCustomAsset* AssetData;

	/* The actual UI list */
	TSharedPtr<STileView<TSharedPtr<FActorData>>> ListViewWidget;

	TSharedPtr<SContentReference> ContentReferencePtr;
	UObject * ContentObject;
};
