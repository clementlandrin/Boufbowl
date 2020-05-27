// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BoufbowlPlayer.generated.h"

class ABoufbowlCamera;
class ABoufbowlPlayerController;
class ABoufbowlCell;
class AAIController;
class UAnimSequence;
class UAnimBlueprint;

UCLASS(Blueprintable)
class ABoufbowlPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ABoufbowlPlayer(const FObjectInitializer& ObjectInitializer);

	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	void BeginPlay();
	AAIController* GetAIController();
	void SetOwnerController(ABoufbowlPlayerController* owner_controller);
	ABoufbowlPlayerController* GetOwnerController();

	void PlayRunAnimation();

	static ABoufbowlPlayer* SpawnPlayer(FString mesh_path, ABoufbowlCell* cell, ABoufbowlPlayerController* owner_controller);
private:
	ABoufbowlPlayerController* m_OwnerController;
	USkeletalMeshComponent* m_MeshComponent;
	AAIController* m_AIController;
	UAnimSequence* m_RunSequence;
};

