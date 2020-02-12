// Fill out your copyright notice in the Description page of Project Settings.

#include "SRTDListview.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/STableViewBase.h"
#include "CustomAssetEditor.h"

#include "ObjectTools.h"
#include "AssetThumbnail.h"
#include "ImageUtils.h"
#include "Widgets/SWidget.h"

void SRTDListview::Construct(const FArguments & Args)
{
	AssetData = Args._ObjectToEdit;

	for (int i = 0; i < AssetData->MeshDataArr.Num(); i++)
	{
		FMeshData ISMData = AssetData->MeshDataArr[i];
		Items.Add(MakeShareable(new FMeshData(ISMData)));
	}

	ChildSlot
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
		[
			SNew(SButton)
			.Text(FText::FromString("Add new list item"))
		.OnClicked(this, &SRTDListview::ButtonPressed)
		]

	+ SScrollBox::Slot()
		[
			SAssignNew(ListViewWidget, STileView<TSharedPtr<FMeshData>>)
			.ItemWidth(64)
		.ItemHeight(64)
		.ListItemsSource(&Items) //The Items array is the source of this listview
		//.OnGenerateRow(this, &SRTDListview::OnGenerateRowForList)
		.OnGenerateTile(this, &SRTDListview::OnGenerateRowForList)
		.OnMouseButtonClick_Raw(this, &SRTDListview::ListItemClick)
		]
		];
}

FReply SRTDListview::ButtonPressed()
{
	//애셋 브라우저와 연결 후 데이터 받아오기

	FMeshData MD;
	MD.MeshData = "StaticMesh'/Engine/BasicShapes/Cube.Cube'";
	MD.ArrayIndex = AssetData->MeshDataArr.Num();

	AssetData->MeshDataArr.Add(MD);
	Items.Add(MakeShareable(new FMeshData(MD)));

	ListViewWidget->RequestListRefresh();

	return FReply::Handled();
}

void SRTDListview::ListItemClick(TSharedPtr<FMeshData> SelectItem)
{
	AssetData->MeshDataIndex = SelectItem->ArrayIndex;
}

TSharedRef<ITableRow> SRTDListview::OnGenerateRowForList(TSharedPtr<FMeshData> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	//fassetthumbnail
	
	//UObject* StaticMesh = LoadObject<UObject>(NULL, *Item.Get()->MeshData, NULL, LOAD_None, NULL);
	UObject* StaticMesh = LoadObject<UObject>(NULL, TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"), NULL, LOAD_None, NULL);
	TSharedPtr<FAssetThumbnailPool> MyThumbnailPool = MakeShareable(new FAssetThumbnailPool(50));
	TSharedPtr<FAssetThumbnail> MyThumbnail = MakeShareable(new FAssetThumbnail(StaticMesh, 64, 64, MyThumbnailPool));

	return
		SNew(STableRow<TSharedPtr<FMeshData>>, OwnerTable)
		.Padding(2.0f)
		[
			MyThumbnail->MakeThumbnailWidget()
		];
}