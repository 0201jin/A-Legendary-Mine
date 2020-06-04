// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PaperSpriteComponent.h"
#include "Components/TextRenderComponent.h"
#include "DataTable/CardDataTable.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CardActor.generated.h"

UCLASS()
class ALEGENDARYMINE_API ACardActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACardActor();

	void SetCardSystem(class ACardSystem* _CardSystem);
	void MoveToNum(int _Num);
	void SetCanClick(bool _Click) { CanClick = _Click; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Rot(float _Ro);
	void RotAndSuffle(float _Ro);

	void MoveSuffle();

	void MoveToUp();

	virtual void NotifyActorOnClicked(FKey PressedButton = EKeys::LeftMouseButton) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetCard(FCardDataTableRow _Buff, FCardDataTableRow _DeBuff);
	TArray<FCardDataTableRow> GetCard();

	void ShowF();
	void ShowB();
	void MoveToSuffle();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UPaperSpriteComponent* FrontSpriteComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UPaperSpriteComponent* BackSpriteComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UTextRenderComponent* Buff;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UTextRenderComponent* Buff_Explain;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UTextRenderComponent* DeBuff;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UTextRenderComponent* DeBuff_Explain;

	TArray<FCardDataTableRow> CardArray;

	FTimerHandle RotTimer;
	FTimerHandle MoveTimer;

	bool CanClick = false;
	bool ShowBig = true;

	class ACardSystem* CardSystem;

	FVector Location;
};
