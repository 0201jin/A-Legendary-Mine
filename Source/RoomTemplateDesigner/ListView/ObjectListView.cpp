// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectListView.h"

void SObjectListView::Construct(const FArguments& Args)
{
	AssetData = Args._ObjectToEdit;

	for (int i = 0; i < AssetData->ActorArr.Num(); i++)
		Items.Add(MakeShareable(new FAData(AssetData->ActorArr[i])));

	ChildSlot
		[
			SNew(SScrollBox)
			+SScrollBox::Slot()
			[
				SAssignNew(ActorListViewWidget, SListView<TSharedPtr<FAData>>)
				.ItemHeight(24)
				.ListItemsSource(&Items)
				.OnMouseButtonClick_Raw(this, &SObjectListView::ListItemClick)
				.OnGenerateRow(this, &SObjectListView::OnGenerateRowForList)
			]
		];
}

void SObjectListView::ListItemClick(TSharedPtr<FAData> SelectItem)
{
	for (int i = 0; i < AssetData->ActorArr.Num(); i++)
	{
		if (SelectItem.Get()->MeshTransform.GetLocation() == AssetData->ActorArr[i].MeshTransform.GetLocation() &&
			SelectItem.Get()->MeshData == AssetData->ActorArr[i].MeshData)
		{
			AssetData->ActorArr.RemoveAt(i);
		}
	}

	RefreshData();
}

void SObjectListView::RefreshData()
{
	Items.Empty();

	for (int i = 0; i < AssetData->ActorArr.Num(); i++)
		Items.Add(MakeShareable(new FAData(AssetData->ActorArr[i])));
}

TSharedRef<ITableRow> SObjectListView::OnGenerateRowForList(TSharedPtr<FAData> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	FString test;
	Item.Get()->MeshData.Split(".", nullptr, &test);

	return
		SNew(STableRow< TSharedPtr<FAData> >, OwnerTable)
		.Padding(2.0f)
		[
			SNew(STextBlock).Text(FText::FromString(*test))
		];
}
