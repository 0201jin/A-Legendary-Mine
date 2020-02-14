// Fill out your copyright notice in the Description page of Project Settings.

#include "ThumbnailWidget.h"
#include "AssetThumbnail.h"
#include "Widgets/SOverlay.h"

void SThumbnailWidget::Construct(const FArguments & Args)
{
	MyThumbnailPool = MakeShareable(new FAssetThumbnailPool(16, false));
	MyThumbnail = MakeShareable(new FAssetThumbnail(Args._Object, 128, 128, MyThumbnailPool));

	ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			[
				MyThumbnail->MakeThumbnailWidget()
			]
		];
}