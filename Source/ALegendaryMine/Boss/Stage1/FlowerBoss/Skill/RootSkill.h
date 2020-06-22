// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Boss/SkillActor.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RootSkill.generated.h"

UCLASS()
class ALEGENDARYMINE_API ARootSkill : public ASkillActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARootSkill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UParticleSystemComponent* Effect;
	UStaticMeshComponent* Cylinder;
};
