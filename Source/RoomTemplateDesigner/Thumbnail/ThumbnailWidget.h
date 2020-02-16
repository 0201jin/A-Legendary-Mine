// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "SlateFwd.h"
#include "UObject/GCObject.h"
#include "SlateBasics.h"
#include "Widgets/SWidget.h"


class SThumbnailWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SThumbnailWidget) {}
	SLATE_ARGUMENT(UObject*, Object)
	SLATE_END_ARGS()

	void Construct(const FArguments & Args);

protected:
	TSharedPtr<class FAssetThumbnailPool> MyThumbnailPool;
	TSharedPtr<class FAssetThumbnail> MyThumbnail;
};
