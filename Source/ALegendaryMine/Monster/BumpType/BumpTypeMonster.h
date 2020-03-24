// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Monster/MonsterActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BumpTypeMonster.generated.h"

UCLASS()
class ALEGENDARYMINE_API ABumpTypeMonster : public AMonsterActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABumpTypeMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
