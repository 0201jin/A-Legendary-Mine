// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "MyCustomAsset.h"

class UMyCustomAsset;
class FCustomAssetEditor;

class SRTDListview : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRTDListview) {}
	SLATE_ARGUMENT(UMyCustomAsset*, ObjectToEdit)
	SLATE_END_ARGS()

	void Construct(const FArguments& Args);
	FReply ButtonPressed();
	void ListItemClick(TSharedPtr<FMeshData> SelectItem);

	/* Adds a new textbox with the string to the list */
	TSharedRef<class ITableRow> OnGenerateRowForList(TSharedPtr<FMeshData> Item, const TSharedRef<class STableViewBase>& OwnerTable);

	/* The list of strings */
	TArray<TSharedPtr<FMeshData>> Items;

	UMyCustomAsset* AssetData;

	/* The actual UI list */
	TSharedPtr<STileView<TSharedPtr<FMeshData>>> ListViewWidget;
};
