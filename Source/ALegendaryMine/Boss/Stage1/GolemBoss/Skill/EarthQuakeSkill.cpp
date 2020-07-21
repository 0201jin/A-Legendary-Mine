// Fill out your copyright notice in the Description page of Project Settings.


#include "EarthQuakeSkill.h"

AEarthQuakeSkill::AEarthQuakeSkill()
{
	PrimaryActorTick.bCanEverTick = true;

	static UStaticMesh* CylinderMesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	static UStaticMesh* CircleMesh = LoadObject<UStaticMesh>(NULL, TEXT("StaticMesh'/Game/Boss/1Stage/Golem/Skill/MEsh/Circle.Circle'"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Cylinder = NewObject<UStaticMeshComponent>(this, TEXT("Cylinder"));
	Cylinder->SetStaticMesh(CylinderMesh);
	Cylinder->AttachTo(RootComponent);
	Cylinder->SetVisibility(false);
	Cylinder->SetGenerateOverlapEvents(true);
	Cylinder->OnComponentBeginOverlap.AddDynamic(this, &AEarthQuakeSkill::OnOverlapBegin);
	Cylinder->SetCollisionProfileName("OverlapAll");

	StunMesh = NewObject<UStaticMeshComponent>(this, TEXT("StunMesh"));
	StunMesh->SetStaticMesh(CylinderMesh);
	StunMesh->AttachTo(RootComponent);
	StunMesh->SetVisibility(false);
	StunMesh->SetGenerateOverlapEvents(true);
	StunMesh->OnComponentBeginOverlap.AddDynamic(this, &AEarthQuakeSkill::OnOverlapBeginStun);
	StunMesh->SetCollisionProfileName("OverlapAll");
	StunMesh->SetRelativeScale3D(FVector(1, 1, 1));

	CircleMeshComp = NewObject<UStaticMeshComponent>(this, TEXT("CircleMesh"));
	CircleMeshComp->SetStaticMesh(CircleMesh);
	CircleMeshComp->AttachTo(StunMesh);
	CircleMeshComp->SetCollisionProfileName("OverlapAll");
	CircleMeshComp->SetRelativeScale3D(FVector(0.5, 0.5, 0.5));
}

void AEarthQuakeSkill::BeginPlay()
{
	Super::BeginPlay();

	Cylinder->SetGenerateOverlapEvents(true);
	StunMesh->SetGenerateOverlapEvents(true);
}

void AEarthQuakeSkill::OnOverlapBeginStun(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerPawn>(OtherActor))
	{
		if (Cast<APlayerPawn>(OtherActor)->GetCapsuleComponent() == OtherComp)
			Cast<APlayerPawn>(OtherActor)->Stun(2);
	}
}

void AEarthQuakeSkill::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	fTime += DeltaTime;

	float Time = 25;
	StunMesh->SetRelativeScale3D(FVector(Time * fTime, Time * fTime, 1));
}
