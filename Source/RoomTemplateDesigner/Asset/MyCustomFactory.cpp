// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCustomFactory.h"
#include "MyCustomAsset.h"

UMyCustomFactory::UMyCustomFactory()
{
	bCreateNew = true;
	bEditAfterNew = true;
	SupportedClass = UMyCustomAsset::StaticClass();
}

UObject* UMyCustomFactory::FactoryCreateNew(UClass* Class, UObject* InParent, FName Name, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	UMyCustomAsset* MyCustomAsset = NewObject<UMyCustomAsset>(InParent, Class, Name, Flags);
	return MyCustomAsset;
}
