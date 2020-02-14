// Fill out your copyright notice in the Description page of Project Settings.

#include "SRTDListview.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/STableViewBase.h"
#include "CustomAssetEditor.h"

#include "ObjectTools.h"
#include "AssetThumbnail.h"
#include "ImageUtils.h"
#include "Widgets/SWidget.h"
#include "SAssetDropTarget.h"
#include "Thumbnail/ThumbnailWidget.h"

void SRTDListview::Construct(const FArguments & Args)
{
	AssetData = Args._ObjectToEdit;

	for (int i = 0; i < AssetData->MeshDataArr.Num(); i++)
	{
		FMeshData ISMData = AssetData->MeshDataArr[i];
		Items.Add(MakeShareable(new FMeshData(ISMData)));
	}

	ContentReferencePtr = SNew(SContentReference)
		.AllowedClass(UStaticMesh::StaticClass())
		.AllowSelectingNewAsset(true)
		.AllowClearingReference(true)
		.ShowFindInBrowserButton(true)
		.AssetReference(this, &SRTDListview::GetFreference)
		.ShowToolsButton(false)
		.WidthOverride(250.0f)
		.InitialAssetViewType(EAssetViewType::Tile)
		.OnSetReference(this, &SRTDListview::SetReference);

	ChildSlot
		[
			SNew(SScrollBox)
			+ SScrollBox::Slot()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		[
			SNew(SAssetDropTarget)
			[
				ContentReferencePtr.ToSharedRef()
			]
		]
	+ SHorizontalBox::Slot()
		[
			SNew(SButton)
			.Text(FText::FromString("Add new list item"))
		.OnClicked(this, &SRTDListview::ButtonPressed)
		]
		]

	+ SScrollBox::Slot()
		[
			SAssignNew(ListViewWidget, STileView<TSharedPtr<FMeshData>>)
			.ItemWidth(128)
		.ItemHeight(148)
		.ListItemsSource(&Items)
		.OnGenerateTile(this, &SRTDListview::OnGenerateRowForList)
		.OnMouseButtonClick_Raw(this, &SRTDListview::ListItemClick)
		]
		];
}

FReply SRTDListview::ButtonPressed()
{
	//애셋 브라우저와 연결 후 데이터 받아오기
	
	if (ContentObject != nullptr)
	{
		FMeshData MD;
		MD.MeshData = ContentObject->GetPathName();
		MD.ArrayIndex = AssetData->MeshDataArr.Num();

		AssetData->MeshDataArr.Add(MD);
		Items.Add(MakeShareable(new FMeshData(MD)));

		ListViewWidget->RequestListRefresh();

		UE_LOG(LogTemp, Log, TEXT("%s"), *ContentObject->GetPathName());
	}

	return FReply::Handled();
}

void SRTDListview::SetReference(UObject * Object)
{
	ContentObject = Object;
}

UObject * SRTDListview::GetFreference() const
{
	return ContentObject;
}

void SRTDListview::ListItemClick(TSharedPtr<FMeshData> SelectItem)
{
	AssetData->MeshDataIndex = SelectItem->ArrayIndex;
}

TSharedRef<ITableRow> SRTDListview::OnGenerateRowForList(TSharedPtr<FMeshData> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	UObject* StaticMesh = LoadObject<UObject>(NULL, *Item.Get()->MeshData, NULL, LOAD_None, NULL);

	return
		SNew(STableRow<TSharedPtr<FMeshData>>, OwnerTable)
		.Padding(2.0f)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.FillHeight(128)
				[	
					SNew(SThumbnailWidget)
					.Object(StaticMesh)
				]
			+ SVerticalBox::Slot()
			.FillHeight(20)
				[
					SNew(STextBlock)
					.Text(FText::FromString(StaticMesh->GetName()))
				]
		];
}