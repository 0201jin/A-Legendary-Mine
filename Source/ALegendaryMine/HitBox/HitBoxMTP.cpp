// Fill out your copyright notice in the Description page of Project Settings.


#include "HitBoxMTP.h"
#include "Player/PlayerPawn.h"
#include "InteractionActor/InteractionActor.h"

// Sets default values
AHitBoxMTP::AHitBoxMTP()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = NewObject<UBoxComponent>(this, TEXT("CollisionBox"));
	RootComponent = Collision;
	Collision->SetCollisionProfileName("OverlapAll");
	Collision->SetRelativeLocation(FVector(0, 0, 0));
	Collision->SetRelativeRotation(FRotator(0, 0, 0));
	Collision->InitBoxExtent(FVector(1, 1, 50));
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AHitBoxMTP::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AHitBoxMTP::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHitBoxMTP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHitBoxMTP::SetDamage(int _Damage)
{
	Damage = _Damage;
}

void AHitBoxMTP::ClearHitPlayer()
{
	HitPlayerList.Empty();
}

void AHitBoxMTP::SetCollisionEnable(ECollisionEnabled::Type _Collision)
{
	Collision->SetCollisionEnabled(_Collision);
}

void AHitBoxMTP::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Damage != 0)
	{
		APlayerPawn* Player = Cast<APlayerPawn>(OtherActor);
		AInteractionActor* InteractionAT = Cast<AInteractionActor>(OtherActor);

		if (Player)
		{
			bool bCheck = false;
			for (int i = 0; i < HitPlayerList.Num(); i++)
			{
				if (HitPlayerList[i] == OtherActor)
					bCheck = true;
			}

			if (!bCheck)
			{
				UE_LOG(LogTemp, Log, TEXT("Attack Success Monster To Player"));
				Player->Damage(Damage);
				HitPlayerList.Add(OtherActor);
			}
		}
		else if (InteractionAT)
		{
			InteractionAT->Damaged(2, GetOwnerActorLocation());
		}
	}
}

