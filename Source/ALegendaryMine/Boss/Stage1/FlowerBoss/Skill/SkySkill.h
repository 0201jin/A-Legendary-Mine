// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Boss/SkillActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SkySkill.generated.h"

UCLASS()
class ALEGENDARYMINE_API ASkySkill : public ASkillActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkySkill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
