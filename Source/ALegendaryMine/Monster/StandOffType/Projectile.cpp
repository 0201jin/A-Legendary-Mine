// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Player/PlayerPawn.h"
#include "Monster/MonsterActor.h"

// Sets default values
AProjectile::AProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	Collision = NewObject<UBoxComponent>(this, TEXT("CollisionBox"));
	Collision->SetCollisionProfileName("OverlapAll");
	Collision->InitBoxExtent(FVector(1, 1, 1));
	Collision->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ProjectileMesh = NewObject<UStaticMeshComponent>(this, TEXT("Projectile"));
	ProjectileMesh->SetCollisionProfileName("OverlapAll");
	ProjectileMesh->AttachToComponent(Collision, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldOffset(GetActorForwardVector() * Speed * DeltaTime);
}

void AProjectile::SetData(UStaticMesh* _Mesh, float _Speed)
{
	ProjectileMesh->SetStaticMesh(_Mesh);
	Damage = 1;
	Speed = _Speed;

	Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnOverlapBegin);
}

void AProjectile::SetSpeed(float _Speed)
{
	Speed = _Speed;
}

void AProjectile::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<APlayerPawn>(OtherActor))
	{
		Cast<APlayerPawn>(OtherActor)->Damage(Damage);
	}

	if (!Cast<AMonsterActor>(OtherActor) && !Cast<AProjectile>(OtherActor))
	{
		Destroy();
	}
}

