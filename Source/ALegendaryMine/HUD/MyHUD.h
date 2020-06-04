// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class ALEGENDARYMINE_API AMyHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AMyHUD();
	void InitHUD();

	void HideAllWidget();
	void InGameWidgetMode();
	void CardSelectWidgetMode();

	void CardSelectWidgetChangeButton(int _ButtonSwitch);

protected:
	virtual void BeginPlay() override;

protected:
	class UClass * InGameWidgetClass;
	class UInGameWidget * InGameWidget;

	class UClass* CardSelectWidgetClass;
	class UCardSelectWidget* CardSelectWidget;
};
