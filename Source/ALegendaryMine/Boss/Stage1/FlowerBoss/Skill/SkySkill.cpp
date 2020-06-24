// Fill out your copyright notice in the Description page of Project Settings.


#include "SkySkill.h"

// Sets default values
ASkySkill::ASkySkill()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static UMaterial* RootMat = LoadObject<UMaterial>(NULL, TEXT("Material'/Game/Boss/1Stage/Flower/Skill/DefaultMaterial.DefaultMaterial'"));
	static UStaticMesh* CylinderMesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Cylinder = NewObject<UStaticMeshComponent>(this, TEXT("Cylinder"));
	Cylinder->SetStaticMesh(CylinderMesh);
	Cylinder->AttachTo(RootComponent);
	Cylinder->SetVisibility(false);
	Cylinder->SetGenerateOverlapEvents(false);
	Cylinder->SetRelativeScale3D(FVector(0.1, 0.1, 0.1));
	Cylinder->OnComponentBeginOverlap.AddDynamic(this, &ASkySkill::OnOverlapBegin);
	Cylinder->SetCollisionProfileName("OverlapAll");

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectAsset(TEXT("ParticleSystem'/Game/Boss/1Stage/Flower/Skill/SkyAttackEffect.SkyAttackEffect'"));

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	Effect->AttachTo(RootComponent);
	Effect->SetTemplate(EffectAsset.Object);
	Effect->SetRelativeLocation(FVector(0, 0, 0));
	Effect->SetActive(false);
}

// Called when the game starts or when spawned
void ASkySkill::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASkySkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Time += DeltaTime;

	if (Time >= 1 && !bCanDamage)
	{
		Cylinder->SetGenerateOverlapEvents(true);
		Cylinder->SetRelativeScale3D(FVector(1, 1, 1));
		bCanDamage = true;
	}
}

