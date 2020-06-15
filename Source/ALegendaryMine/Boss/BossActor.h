// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/PlayerController.h"
#include "AIController.h"
#include "Engine.h"
#include "Struct/MapGeneratorStruct.h"
#include "MyGameInstance.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BossActor.generated.h"

UCLASS()
class ALEGENDARYMINE_API ABossActor : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABossActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController);

public:	
	virtual void Damage(int _Damage, AActor* _ACKActor);

protected:
	AAIController* AiController;

	int Health;
};
