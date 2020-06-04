// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "InGameWidget.h"
#include "CardSelectWidget.h"
#include "Engine.h"
#include "Player/MyPlayerController.h"

AMyHUD::AMyHUD()
{
	static ConstructorHelpers::FClassFinder<UInGameWidget> InGameUIClass(TEXT("WidgetBlueprint'/Game/Widget/InGameWidget/InGameWidget'"));
	InGameWidgetClass = InGameUIClass.Class;

	static ConstructorHelpers::FClassFinder<UCardSelectWidget> CardSelectUIClass(TEXT("WidgetBlueprint'/Game/Widget/InGameWidget/CardSelectWidget'"));
	CardSelectWidgetClass = CardSelectUIClass.Class;
}

void AMyHUD::InitHUD()
{
	InGameWidget = CreateWidget<UInGameWidget>(GetOwningPlayerController(), InGameWidgetClass);
	InGameWidget->AddToViewport();
	InGameWidget->SetVisibility(ESlateVisibility::Hidden);

	CardSelectWidget = CreateWidget<UCardSelectWidget>(GetOwningPlayerController(), CardSelectWidgetClass);
	CardSelectWidget->AddToViewport();
	CardSelectWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AMyHUD::HideAllWidget()
{
	InGameWidget->SetVisibility(ESlateVisibility::Hidden);
	CardSelectWidget->SetVisibility(ESlateVisibility::Hidden);
}

void AMyHUD::InGameWidgetMode()
{
	HideAllWidget();
	InGameWidget->SetVisibility(ESlateVisibility::Visible);
	Cast<AMyPlayerController>(GetOwningPlayerController())->SetFightMode(true);
}

void AMyHUD::CardSelectWidgetMode()
{
	HideAllWidget();
	CardSelectWidget->SetVisibility(ESlateVisibility::Visible);
	Cast<AMyPlayerController>(GetOwningPlayerController())->SetFightMode(false);
}

void AMyHUD::CardSelectWidgetChangeButton(int _ButtonSwitch)
{
	CardSelectWidget->CardSelectWidgetChangeButton(_ButtonSwitch);
}

void AMyHUD::BeginPlay()
{
}
