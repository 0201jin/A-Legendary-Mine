// Fill out your copyright notice in the Description page of Project Settings.


#include "CardSelectWidget.h"
#include "Level/InGame.h"

void UCardSelectWidget::ShuffleCard()
{
	Cast<AInGame>(GetWorld()->GetLevelScriptActor())->ShuffleCard();
}

void UCardSelectWidget::CameraToCharacter()
{
	Cast<AInGame>(GetWorld()->GetLevelScriptActor())->CameraToCharacter();
}

void UCardSelectWidget::CardSelectWidgetChangeButton(int _ButtonSwitch)
{
	UpdateButtonSwitch.Broadcast(_ButtonSwitch);
}
