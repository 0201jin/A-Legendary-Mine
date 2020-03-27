// Fill out your copyright notice in the Description page of Project Settings.

#include "RoomActiveActor.h"
#include "Level/InGame.h"
#include "Player/PlayerPawn.h"

// Sets default values
ARoomActiveActor::ARoomActiveActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collision = NewObject<UBoxComponent>(this, TEXT("CollisionBox"));
	RootComponent = Collision;
	Collision->SetCollisionProfileName("OverlapAll");
	Collision->RelativeLocation = FVector(0, 0, 50);
	Collision->RelativeRotation = FRotator(0, 0, 0);
	Collision->InitBoxExtent(FVector(50, 25, 50));
	Collision->OnComponentBeginOverlap.AddDynamic(this, &ARoomActiveActor::OnOverlapBegin);
}

void ARoomActiveActor::SetRoomNumber(int _RoomNumber)
{
	RoomNumber = _RoomNumber;
}

// Called when the game starts or when spawned
void ARoomActiveActor::BeginPlay()
{
	Super::BeginPlay();

}

void ARoomActiveActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (RoomNumber != -1 && Cast<APlayerPawn>(OtherActor) != nullptr)
	{
		Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		//모든 방의 창을 닫는 함수
		Cast<AInGame>(GetWorld()->GetLevelScriptActor())->GetMapgen()->RoomActiveActor(RoomNumber);
	}
}
