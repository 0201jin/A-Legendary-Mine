// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillActor.h"
#include "Engine.h"

// Sets default values
ASkillActor::ASkillActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASkillActor::BeginPlay()
{
	Super::BeginPlay();
	
	Effect->SetActive(true);
}

void ASkillActor::DestroySkill()
{
	Destroy();
}

void ASkillActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerPawn>(OtherActor))
	{
		if (Cast<APlayerPawn>(OtherActor)->GetCapsuleComponent() == OtherComp)
			Cast<APlayerPawn>(OtherActor)->Damage(1);
	}
}

// Called every frame
void ASkillActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkillActor::SetLifeTime(float _Time)
{
	GetWorldTimerManager().SetTimer(LifeTime, this, &ASkillActor::DestroySkill, 0.1, false, _Time);
}

