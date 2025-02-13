// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Factories/Factory.h"
#include "MyCustomFactory.generated.h"

/**
 * 
 */
UCLASS()
class ROOMTEMPLATEDESIGNER_API UMyCustomFactory : public UFactory
{
	GENERATED_BODY()
	
public:
	UMyCustomFactory();

	virtual UObject* FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
