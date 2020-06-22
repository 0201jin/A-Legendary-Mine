// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkillActor.generated.h"

UCLASS()
class ALEGENDARYMINE_API ASkillActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void DestroySkill();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetLifeTime(float _Time);

protected:
	FTimerHandle LifeTime;
};
