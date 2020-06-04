// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CardSelectWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateButtonSwitch ,int32 , _ButtonSwitch);

UCLASS()
class ALEGENDARYMINE_API UCardSelectWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void ShuffleCard();

	UFUNCTION(BlueprintCallable, Category = "UMG Function")
		void CameraToCharacter();

	void CardSelectWidgetChangeButton(int _ButtonSwitch);

protected:
	UPROPERTY(BluePrintAssignable, Category = "UpDateNavMesh")
		FUpdateButtonSwitch UpdateButtonSwitch;
};
