// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "System/MapGeneratorSys/MapGeneratorSys.h"
#include "MyGameInstance.h"

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "InGame.generated.h"

/**
 * 
 */
UCLASS()
class ALEGENDARYMINE_API AInGame : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:
	AInGame();
	~AInGame();

	UMyGameInstance * GetMyGameInstance();

protected:
	virtual void BeginPlay();

protected:
	MapGeneratorSys * Mapgen;
	UMyGameInstance * MyGameInstance;
};
