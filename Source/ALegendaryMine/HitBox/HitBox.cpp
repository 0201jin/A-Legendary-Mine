// Fill out your copyright notice in the Description page of Project Settings.

#include "HitBox.h"
#include "Monster/MonsterActor.h"

// Sets default values
AHitBox::AHitBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = NewObject<UBoxComponent>(this, TEXT("CollisionBox"));
	RootComponent = Collision;
	Collision->SetCollisionProfileName("OverlapAll");
	Collision->RelativeLocation = FVector(0, 0, 0);
	Collision->RelativeRotation = FRotator(0, 0, 0);
	Collision->InitBoxExtent(FVector(25, 1, 1));
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AHitBox::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AHitBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHitBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHitBox::SetDamage(int _Damage)
{
	Damage = _Damage;
}

void AHitBox::ClearHitMonster()
{
	HitMonsterList.Empty();
}

void AHitBox::SetCollisionEnable(ECollisionEnabled::Type _Collision)
{
	Collision->SetCollisionEnabled(_Collision);
}

void AHitBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Damage != 0)
	{
		AMonsterActor* Monster = Cast<AMonsterActor>(OtherActor);

		if (Monster)
		{
			bool bCheck = false;
			for (int i = 0; i < HitMonsterList.Num(); i++)
			{
				if (HitMonsterList[i] == OtherActor)
					bCheck = true;
			}

			if (!bCheck)
			{
				UE_LOG(LogTemp, Log, TEXT("Attack Success"));
				Monster->Damage(Damage, this);
				HitMonsterList.Add(OtherActor);
			}
		}
	}
}