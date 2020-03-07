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

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class USpringArmComponent * CameraBoom;

	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		class UCameraComponent * FollowCamera;

public:
	// Sets default values for this pawn's properties
	APlayerPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void FB_Move(float _value);
	void LR_Move(float _value);
};
