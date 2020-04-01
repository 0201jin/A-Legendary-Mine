// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectListView.h"

void SObjectListView::Construct(const FArguments& Args)
{
	AssetData = Args._ObjectToEdit;

	for (int i = 0; i < AssetData->ActorArr.Num(); i++)
		Items.Add(MakeShareable(new FString(AssetData->ActorArr[i].MeshData)));

	ChildSlot
		[
			SNew(SScrollBox)
			+SScrollBox::Slot()
			[
				SAssignNew(ActorListViewWidget, SListView<TSharedPtr<FString>>)
				.ItemHeight(24)
				.ListItemsSource(&Items)
				.OnGenerateRow(this, &SObjectListView::OnGenerateRowForList)
			]
		];
}

TSharedRef<ITableRow> SObjectListView::OnGenerateRowForList(TSharedPtr<FString> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	FString test;
	Item.Get()->Split(".", nullptr, &test);

	return
		SNew(STableRow< TSharedPtr<FString> >, OwnerTable)
		.Padding(2.0f)
		[
			SNew(STextBlock).Text(FText::FromString(*test))
		];
}
