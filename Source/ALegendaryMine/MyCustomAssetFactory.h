// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "MyCustomAssetFactory.generated.h"

/**
 * 
 */
UCLASS()
class ALEGENDARYMINE_API UMyCustomAssetFactory : public UFactory
{
	GENERATED_BODY()

public:
	UMyCustomAssetFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
