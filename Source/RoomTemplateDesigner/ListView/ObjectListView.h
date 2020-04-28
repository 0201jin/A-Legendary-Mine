// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SlateBasics.h"
#include "MyCustomAsset.h"
#include "WorkflowOrientedApp/SContentReference.h"

class UMyCustomAsset;
class FCustomAssetEditor;

class SObjectListView : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SObjectListView) {}
	SLATE_ARGUMENT(UMyCustomAsset*, ObjectToEdit)
	SLATE_END_ARGS()

	void Construct(const FArguments& Args);

	void ListItemClick(TSharedPtr<FAData> SelectItem);

	void RefreshData();

	TSharedRef<ITableRow> OnGenerateRowForList(TSharedPtr<FAData> Item, const TSharedRef<STableViewBase>& OwnerTable);

	TSharedPtr<SListView<TSharedPtr<FAData>>> ActorListViewWidget;
	TArray<TSharedPtr<FAData>> Items;

	UMyCustomAsset* AssetData;
};
