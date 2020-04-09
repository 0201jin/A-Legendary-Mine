// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitBoxMTP.generated.h"

UCLASS()
class ALEGENDARYMINE_API AHitBoxMTP : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHitBoxMTP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetDamage(int _Damage);
	void ClearHitPlayer();
	void SetCollisionEnable(ECollisionEnabled::Type _Collision);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UBoxComponent* Collision;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	TArray<AActor*> HitPlayerList;

	int Damage = 0;
};
