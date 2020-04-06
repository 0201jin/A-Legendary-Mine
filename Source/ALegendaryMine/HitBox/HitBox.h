// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Components/BoxComponent.h"
#include "Engine.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitBox.generated.h"

UCLASS()
class ALEGENDARYMINE_API AHitBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDamage(int _Damage);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UBoxComponent* Collision;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	TArray<AActor*> HitMonsterList;

	int Damage = 0;
};
