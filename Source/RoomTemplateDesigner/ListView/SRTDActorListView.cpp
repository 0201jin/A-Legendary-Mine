// Fill out your copyright notice in the Description page of Project Settings.


#include "SRTDActorListView.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Views/STableViewBase.h"
#include "CustomAssetEditor.h"

#include "ObjectTools.h"
#include "AssetThumbnail.h"
#include "ImageUtils.h"
#include "Widgets/SWidget.h"
#include "SAssetDropTarget.h"
#include "Thumbnail/ThumbnailWidget.h"

void SRTDActorListView::Construct(const FArguments & Args)
{
	AssetData = Args._ObjectToEdit;

	for (int i = 0; i < AssetData->ActorDataArr.Num(); i++)
	{
		FActorData ActorData = AssetData->ActorDataArr[i];
		Items.Add(MakeShareable(new FActorData(ActorData)));
	}

	ContentReferencePtr = SNew(SContentReference)
		.AllowedClass(UBlueprintCore::StaticClass())
		.AllowSelectingNewAsset(true)
		.AllowClearingReference(true)
		.ShowFindInBrowserButton(true)
		.AssetReference(this, &SRTDActorListView::GetFreference)
		.ShowToolsButton(false)
		.WidthOverride(250.0f)
		.InitialAssetViewType(EAssetViewType::Tile)
		.OnSetReference(this, &SRTDActorListView::SetReference);

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
		.OnClicked(this, &SRTDActorListView::ButtonPressed)
		]
		]

	+ SScrollBox::Slot()
		[
			SAssignNew(ListViewWidget, STileView<TSharedPtr<FActorData>>)
			.ItemWidth(128)
		.ItemHeight(148)
		.ListItemsSource(&Items)
		.OnGenerateTile(this, &SRTDActorListView::OnGenerateRowForList)
		.OnMouseButtonClick_Raw(this, &SRTDActorListView::ListItemClick)
		]
		];
}

FReply SRTDActorListView::ButtonPressed()
{
	//애셋 브라우저와 연결 후 데이터 받아오기

	if (ContentObject != nullptr)
	{
		FActorData MD;
		MD.MeshData = ContentObject->GetPathName();
		MD.ArrayIndex = AssetData->ActorDataArr.Num();

		AssetData->ActorDataArr.Add(MD);
		Items.Add(MakeShareable(new FActorData(MD)));

		ListViewWidget->RequestListRefresh();

		UE_LOG(LogTemp, Log, TEXT("%s"), *ContentObject->GetPathName());
	}

	return FReply::Handled();
}

void SRTDActorListView::SetReference(UObject * Object)
{
	ContentObject = Object;
}

UObject * SRTDActorListView::GetFreference() const
{
	return ContentObject;
}

void SRTDActorListView::ListItemClick(TSharedPtr<FActorData> SelectItem)
{
	AssetData->ActorDataIndex = SelectItem->ArrayIndex;
	AssetData->bSelectActor = true;
}

TSharedRef<ITableRow> SRTDActorListView::OnGenerateRowForList(TSharedPtr<FActorData> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	UObject* StaticMesh = LoadObject<UObject>(NULL, *Item.Get()->MeshData, NULL, LOAD_None, NULL);

	return
		SNew(STableRow<TSharedPtr<FActorData>>, OwnerTable)
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