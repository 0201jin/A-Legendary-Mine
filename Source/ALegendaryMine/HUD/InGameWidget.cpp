// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameWidget.h"
#include "Level/InGame.h"

void UInGameWidget::GenerateRoom()
{
	Cast<AInGame>(GetWorld()->GetLevelScriptActor())->GenerateMap();
}