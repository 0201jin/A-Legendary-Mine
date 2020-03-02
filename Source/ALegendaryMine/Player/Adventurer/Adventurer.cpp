// Fill out your copyright notice in the Description page of Project Settings.


#include "Adventurer.h"

// Sets default values
AAdventurer::AAdventurer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->MaxWalkSpeed = 300;
}

// Called when the game starts or when spawned
void AAdventurer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAdventurer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}