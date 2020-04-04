// Fill out your copyright notice in the Description page of Project Settings.


#include "Adventurer.h"
#include "MyGameInstance.h"

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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> JumpMo(TEXT("AnimMontage'/Game/Character/Adventurer/Animation/Adventurer_Armature_Jump_Montage.Adventurer_Armature_Jump_Montage'"));
	JumpAnimation = JumpMo.Object;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> AttackMo(TEXT("AnimMontage'/Game/Character/Adventurer/Animation/Adventurer_Armature_Attack_Sword_Montage.Adventurer_Armature_Attack_Sword_Montage'"));
	AttackAnimation = AttackMo.Object;
}

// Called when the game starts or when spawned
void AAdventurer::BeginPlay()
{
	Super::BeginPlay();

	Weapon->SetStaticMesh(Cast<UMyGameInstance>(GetWorld()->GetGameInstance())->WeaponData[0].WeaponMesh);
}

// Called every frame
void AAdventurer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}