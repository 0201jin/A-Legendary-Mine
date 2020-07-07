// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Player/PlayerPawn.h"
#include "Engine/Classes/Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

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

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetLifeTime(float _Time);

protected:
	FTimerHandle LifeTime;

	bool bCanDamage = false;
	float Time = 0;

protected:
	UNiagaraComponent* Effect;

	UStaticMeshComponent* Cylinder;
};
