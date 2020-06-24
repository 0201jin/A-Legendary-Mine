// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Engine.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CircleSkillRange.generated.h"

UCLASS()
class ALEGENDARYMINE_API ACircleSkillRange : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACircleSkillRange();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetLifeTime(float _Time, UClass * _SpawnActor, float _SkillLifeTime = 1);
	void SetLifeTime(float _Time, float _SpawnTime, UClass * _SpawnActor, float _SkillLifeTime = 1);

protected:
	UParticleSystemComponent* Effect;
	UClass* SpawnActor;

	float Time = 0;
	float LifeTime = 0;
	float SpawnTime = 0;
	float SkillLifeTime = 0;

	bool bSpawn = false;
};
