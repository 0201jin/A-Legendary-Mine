// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerPawn.generated.h"

UCLASS()
class ALEGENDARYMINE_API APlayerPawn : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UStaticMeshComponent* Weapon;

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

	UFUNCTION(BlueprintCallable)
		void CheckEndAttack();

	UFUNCTION(BlueprintCallable)
		void CheckInputAttack();

	UFUNCTION(BlueprintCallable)
		void MoveAttack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool CheckAction();
	void MoveTimer();
	void EndMoveTimer();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetRot(FRotator _Ro);

	void Attack(FRotator _Ro);

	void FB_Move(float _value);
	void LR_Move(float _value);

	void Jump();
	void JumpTimerFunc();
	void JumpTimerEndFunc();
	void JumpTimerCoolFunc();

	void Damage(int _Damage);

protected:
	float PlayerSpeed;
	int AD;
	int HitBoxX, HitBoxY;

	float fFBValue, fLRValue;

	bool bJump = true;
	bool bJumping = false;
	bool bGodMode = false;

	bool bAttack = false;
	bool bAttackWhenAttack = false;
	int iComboCnt = 0;

	class UAnimMontage* JumpAnimation;
	class UAnimMontage* AttackAnimation;

	FTimerHandle JumpTimer;
	FTimerHandle JumpTimerEnd;

	class AHitBox* HitBox;

	TArray<AActor*> HitMonsterList;
};
