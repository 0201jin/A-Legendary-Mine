// Fill out your copyright notice in the Description page of Project Settings.


#include "BumpTypeMonster.h"

// Sets default values
ABumpTypeMonster::ABumpTypeMonster()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABumpTypeMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABumpTypeMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AiController->MoveToLocation(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
}

