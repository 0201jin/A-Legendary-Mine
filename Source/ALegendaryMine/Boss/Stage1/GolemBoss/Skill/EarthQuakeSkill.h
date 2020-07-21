// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss/SkillActor.h"
#include "EarthQuakeSkill.generated.h"

/**
 * 
 */
UCLASS()
class ALEGENDARYMINE_API AEarthQuakeSkill : public ASkillActor
{
	GENERATED_BODY()
	
public:
	AEarthQuakeSkill();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnOverlapBeginStun(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UStaticMeshComponent* StunMesh;
	UStaticMeshComponent* CircleMeshComp;

	float fTime = 0;
};
