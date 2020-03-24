// Fill out your copyright notice in the Description page of Project Settings.


#include "Adventurer.h"

// Sets default values
AAdventurer::AAdventurer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerSpeed = 270;

	GetCharacterMovement()->MaxWalkSpeed = PlayerSpeed;

	GetCapsuleComponent()->SetCapsuleHalfHeight(32);
	GetCapsuleComponent()->SetCapsuleRadius(32);
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Skeletal(TEXT("SkeletalMesh'/Game/Character/Adventurer/Adventurer.Adventurer'"));
	GetMesh()->SetSkeletalMesh(Skeletal.Object);
	GetMesh()->SetRelativeRotation(FRotator(0, 90, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, 0));
	
	static ConstructorHelpers::FObjectFinder<UClass> AnimClass(TEXT("AnimBlueprint'/Game/Character/Adventurer/Animation/Adventurer_Anim_BP.Adventurer_Anim_BP_C'"));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->AnimClass = AnimClass.Object;
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