// Fill out your copyright notice in the Description page of Project Settings.


#include "RootSkill.h"

// Sets default values
ARootSkill::ARootSkill()
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
	Cylinder->SetGenerateOverlapEvents(true);
	Cylinder->OnComponentBeginOverlap.AddDynamic(this, &ARootSkill::OnOverlapBegin);
	Cylinder->SetCollisionProfileName("OverlapAll");

	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> EffectAsset(TEXT("NiagaraSystem'/Game/Boss/1Stage/Flower/Skill/RootSkill_NG.RootSkill_NG'"));

	Effect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MovementParticles"));
	Effect->AttachTo(RootComponent);
	Effect->SetAsset(EffectAsset.Object);
	Effect->SetRelativeLocation(FVector(0, 0, 0));
	Effect->SetActive(false);
}

// Called when the game starts or when spawned
void ARootSkill::BeginPlay()
{
	Super::BeginPlay();

	Cylinder->SetGenerateOverlapEvents(true);
}

// Called every frame
void ARootSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
