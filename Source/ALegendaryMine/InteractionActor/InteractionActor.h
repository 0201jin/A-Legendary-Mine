// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDamageInteraction, int32, Damage, FVector, Location);

UCLASS()
class ALEGENDARYMINE_API AInteractionActor : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Mesh)
		bool SpawnMonster;
	
public:	
	// Sets default values for this actor's properties
	AInteractionActor();

	void Damaged(int _Damage, FVector _TLo = FVector());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool GetSpawnMonster();

protected:
	UPROPERTY(BluePrintAssignable, Category = "UpDateNavMesh")
		FDamageInteraction DamageInteraction;
};
