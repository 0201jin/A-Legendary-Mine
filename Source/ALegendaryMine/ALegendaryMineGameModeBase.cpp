// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "ALegendaryMineGameModeBase.h"
#include "Player/MyPlayerController.h"
#include "Player/Adventurer/Adventurer.h"
#include "HUD/MyHUD.h"

AALegendaryMineGameModeBase::AALegendaryMineGameModeBase()
{
	DefaultPawnClass = AAdventurer::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	HUDClass = AMyHUD::StaticClass();
	//GameStateClass = AMyGameStateBase::StaticClass();
}