// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUD.h"
#include "InGameWidget.h"
#include "Engine.h"

AMyHUD::AMyHUD()
{
	static ConstructorHelpers::FClassFinder<UInGameWidget> InGameUIClass(TEXT("WidgetBlueprint'/Game/Widget/InGameWidget/InGameWidget'"));
	InGameWidgetClass = InGameUIClass.Class;
}

void AMyHUD::BeginPlay()
{
	InGameWidget = CreateWidget<UInGameWidget>(GetOwningPlayerController(), InGameWidgetClass);
	InGameWidget->AddToViewport();
}
