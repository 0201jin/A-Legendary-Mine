// Fill out your copyright notice in the Description page of Project Settings.


#include "CircleSkillRange.h"
#include "Boss/SkillActor.h"

// Sets default values
ACircleSkillRange::ACircleSkillRange()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectAsset(TEXT("ParticleSystem'/Game/PublicEffect/AttackFromSky/AttackFromSkyEffect.AttackFromSkyEffect'"));

	RootComponent = Effect;
	Effect->SetTemplate(EffectAsset.Object);
	Effect->SetRelativeLocation(FVector(0, 0, 0.1));
}

// Called when the game starts or when spawned
void ACircleSkillRange::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACircleSkillRange::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Time += DeltaTime;

	if (LifeTime <= Time)
	{
		GetWorld()->SpawnActor<ASkillActor>(SpawnActor, GetActorTransform())->SetLifeTime(1);
		Destroy();
	}

	float Result = Time / LifeTime;

	Effect->SetVectorParameter("SizeScale", FVector(Result, Result, 0));
	Effect->SetFloatParameter("ColorAlpha", Result);
}

void ACircleSkillRange::SetLifeTime(float _Time, UClass* _SpawnActor)
{
	LifeTime = _Time;
	SpawnActor = _SpawnActor;
}

