// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "SlateFwd.h"
#include "UObject/GCObject.h"
#include "SlateBasics.h"
#include "Widgets/SWidget.h"

class SRoomSizeWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SRoomSizeWidget) {}
	SLATE_ARGUMENT(TSharedPtr<class SRTDViewport>, Viewport)
	SLATE_END_ARGS()

	void Construct(const FArguments & Args);
	FReply ButtonPressed();

protected:
	TSharedPtr<class SRTDViewport> Viewport;

	int32 x, y;
};
