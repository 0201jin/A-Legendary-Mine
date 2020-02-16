// Fill out your copyright notice in the Description page of Project Settings.


#include "SRoomSizeWidget.h"
#include "ViewPort/SRTDViewport.h"
#include "Widgets/Input/SNumericEntryBox.h"

void SRoomSizeWidget::Construct(const FArguments & Args)
{
	Viewport = Args._Viewport;

	ChildSlot
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
			[
				SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				[
					SNew(SNumericEntryBox<int32>)
					.AllowSpin(true)
					.MinValue(0)
					.MaxValue(9999)
					.Value_Lambda([=]() -> int32 { return x; })
					.OnValueChanged(SNumericEntryBox<int32>::FOnValueChanged::CreateLambda([=](int32 Value)
					{
						x = Value;
					}))
					.OnValueCommitted(SNumericEntryBox<int32>::FOnValueCommitted::CreateLambda([=](int32 Value, ETextCommit::Type CommitType)
					{
						x = Value;
					}))
				]
				+ SVerticalBox::Slot()
				[
					SNew(SNumericEntryBox<int32>)
					.AllowSpin(true)
					.MinValue(0)
					.MaxValue(9999)
					.Value_Lambda([=]() -> int32 { return y; })
					.OnValueChanged(SNumericEntryBox<int32>::FOnValueChanged::CreateLambda([=](int32 Value)
					{
						y = Value;
					}))
					.OnValueCommitted(SNumericEntryBox<int32>::FOnValueCommitted::CreateLambda([=](int32 Value, ETextCommit::Type CommitType)
					{
						y = Value;
					}))
				]
			]
			+ SHorizontalBox::Slot()
			[
				SNew(SButton)
				.Text(FText::FromString("Add new list item"))
				.OnClicked(this, &SRoomSizeWidget::ButtonPressed)
			]
		];
}

FReply SRoomSizeWidget::ButtonPressed()
{
	Viewport->CreateWalls(x, y);
	return FReply::Handled();
}