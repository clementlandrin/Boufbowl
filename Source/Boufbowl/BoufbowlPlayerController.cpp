// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "BoufbowlPlayerController.h"
#include "Boufbowl/BoufbowlCamera.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/DecalComponent.h"
#include "BoufbowlPlayer.h"
#include "Materials/Material.h"
#include "Engine/Classes/Engine/DecalActor.h"
#include "BoufbowlGrid.h"
#include "Engine/World.h"

ABoufbowlPlayerController::ABoufbowlPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		m_DecalMaterial = DecalMaterialAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> MannequinSkeletalMesh(TEXT("SkeletalMesh'/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (MannequinSkeletalMesh.Succeeded())
	{
		m_SkeletalMesh = MannequinSkeletalMesh.Object;
	}
}

void ABoufbowlPlayerController::InitializeDecal()
{
	if (!GetWorld())
		return;
	
	m_DecalActor = GetWorld()->SpawnActor<ADecalActor>(FVector(0.0f, 0.0f, 0.0f), FRotator::ZeroRotator);

	if (!m_DecalActor)
		return;

	float radius = 30.0f;
	m_DecalActor->GetDecal()->DecalSize = FVector(radius, radius, radius);
	m_DecalActor->GetDecal()->FadeScreenSize = 0.0f;

	if (m_DecalMaterial)
	{
		m_DecalActor->SetDecalMaterial(m_DecalMaterial);
	}
}

void ABoufbowlPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitializeDecal();
}

void ABoufbowlPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	
	FHitResult Hit;
	GetHitResultUnderCursor(ECC_Visibility, false, Hit);
	m_HitCursor = Hit;

	if (m_DecalActor)
	{
		FRotator rotation = Hit.ImpactNormal.Rotation();
		m_DecalActor->AddActorWorldRotation(rotation);
		m_DecalActor->GetDecal()->SetWorldLocation(m_HitCursor.Location);
	}
}

void ABoufbowlPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	InputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &ABoufbowlPlayerController::LeftMouseButtonClick);
	//InputComponent->BindAction("RightMouseButton", IE_Released, this, &ABoufbowlPlayerController::RightMouseButtonClick);
	//InputComponent->BindAction("MiddleMouseButton", IE_Pressed, this, &ABoufbowlPlayerController::MiddleMouseButtonClick);
}

void ABoufbowlPlayerController::SpawnPlayer(FVector location)
{
	UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayerController::SpawnPlayer"));

	USkeletalMesh* skeletal_mesh = m_SkeletalMesh;

	if (skeletal_mesh)
	{
		UE_LOG(LogTemp, Log, TEXT("ABOufbowlPlayerController::SpawnPlayer found skeletal mesh"))
		FVector BoundsBoxExtent = skeletal_mesh->GetBounds().BoxExtent;
		location.Z += BoundsBoxExtent.Z / 2 + 10.0f;
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayerController::SpawnPlayer no skeletal mesh"));
	}

	FTransform actual_spawn_transform = FTransform(FRotator(0.0f, 0.0f, 0.0f), location);

	FActorSpawnParameters spawn_parameters;
	spawn_parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ABoufbowlPlayer* spawned_player = GetWorld()->SpawnActor<ABoufbowlPlayer>(
		ABoufbowlPlayer::StaticClass(), actual_spawn_transform, spawn_parameters);
}

void ABoufbowlPlayerController::LeftMouseButtonClick()
{
	UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayerController::LeftMouseButtonClick"));

	if (m_HitCursor.bBlockingHit)
	{
		UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayerController::LeftMouseButtonClick hit something"));

		SpawnPlayer(m_HitCursor.Location);

		if (m_BoufbowlGrid)
		{
			FIntVector cell_id = m_BoufbowlGrid->GetCellIdFromLocation(m_HitCursor.Location);

			UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayerController::LeftMouseButtonClick clicked on cell : %d, %d"), cell_id.X, cell_id.Y);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ABoufbowlPlayerController::LeftMouseButtonClick m_BoufbowlGrid is nullptr"));
		}
	}
}

void ABoufbowlPlayerController::MoveToMouseCursor()
{	
	if (m_HitCursor.bBlockingHit)
	{
		// We hit something, move there
		SetNewMoveDestination(m_HitCursor.ImpactPoint);
	}
}

void ABoufbowlPlayerController::SetNewMoveDestination(const FVector DestLocation)
{
	APawn* const MyPawn = GetPawn();
	if (MyPawn)
	{
		float const Distance = FVector::Dist(DestLocation, MyPawn->GetActorLocation());

		// We need to issue move command only if far enough in order for walk animation to play correctly
		if ((Distance > 120.0f))
		{
			UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, DestLocation);
		}
	}
}

void ABoufbowlPlayerController::SetGrid(ABoufbowlGrid* boufbowl_grid)
{
	m_BoufbowlGrid = boufbowl_grid;
}