// Fill out your copyright notice in the Description page of Project Settings.


#include "RootSkill.h"

#include "Player/PlayerPawn.h"

// Sets default values
ARootSkill::ARootSkill()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static UStaticMesh* RootMesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Boss/1Stage/Flower/Skill/Cone.Cone'"));
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

	static ConstructorHelpers::FObjectFinder<UParticleSystem> EffectAsset(TEXT("ParticleSystem'/Game/Boss/1Stage/Flower/Skill/RootSkillEffect.RootSkillEffect'"));

	Effect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovementParticles"));
	Effect->AttachTo(RootComponent);
	Effect->SetTemplate(EffectAsset.Object);
	Effect->SetRelativeLocation(FVector(0, 0, 0));
	Effect->SetActive(false);
}

// Called when the game starts or when spawned
void ARootSkill::BeginPlay()
{
	Super::BeginPlay();

	Effect->SetActive(true);
}

void ARootSkill::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerPawn>(OtherActor))
	{
		if (Cast<APlayerPawn>(OtherActor)->GetCapsuleComponent() == OtherComp)
			Cast<APlayerPawn>(OtherActor)->Damage(1);
	}
}

// Called every frame
void ARootSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
