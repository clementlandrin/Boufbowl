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
#include "BoufbowlCell.h"
#include "BoufbowlPlayer.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Engine/World.h"
#include "AIController.h"

ABoufbowlPlayerController::ABoufbowlPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;

	ConstructorHelpers::FObjectFinder<UMaterial> DecalMaterialAsset(TEXT("Material'/Game/TopDownCPP/Blueprints/M_Cursor_Decal.M_Cursor_Decal'"));
	if (DecalMaterialAsset.Succeeded())
	{
		m_DecalMaterial = DecalMaterialAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterial> BaseCellMaterialAsset(TEXT("Material'/Game/Geometry/Meshes/CubeMaterial.CubeMaterial'"));
	if (BaseCellMaterialAsset.Succeeded())
	{
		m_BaseCellMaterial = BaseCellMaterialAsset.Object;
	}

	ConstructorHelpers::FObjectFinder<UMaterial> SelectionMaterialAsset(TEXT("Material'/Game/Geometry/Meshes/SelectionMaterial.SelectionMaterial'"));
	if (SelectionMaterialAsset.Succeeded())
	{
		m_SelectionMaterial = SelectionMaterialAsset.Object;
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
	InputComponent->BindAction("RightMouseButton", IE_Released, this, &ABoufbowlPlayerController::RightMouseButtonClick);
	//InputComponent->BindAction("MiddleMouseButton", IE_Pressed, this, &ABoufbowlPlayerController::MiddleMouseButtonClick);
}

ABoufbowlPlayer* ABoufbowlPlayerController::SpawnPlayer(FVector location)
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

	return spawned_player;
}

void ABoufbowlPlayerController::SpawnPlayerSelectedCell()
{
	if (!m_SelectedCell->GetBoufbowlPlayer())
	{
		ABoufbowlPlayer* spawned_player = SpawnPlayer(m_SelectedCell->GetLocation());

		if (spawned_player)
		{
			m_SelectedCell->SetBoufbowlPlayer(spawned_player);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ABoufbowlPlayerController::LeftMouseButtonClick couldn't spawn player"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ABoufbowlPlayerController::LeftMouseButtonClick this cell already contains a player"));
	}
}

void ABoufbowlPlayerController::MovePlayerToCell(ABoufbowlCell* cell)
{
	if (m_SelectedCell && m_SelectedCell->GetBoufbowlPlayer())
	{
		UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayerController::MovePlayerToCell moving player to destination cell"));
		ABoufbowlPlayer* boufbowl_player = m_SelectedCell->GetBoufbowlPlayer();
		AAIController* ai_controller = boufbowl_player->GetAIController();
		ai_controller->MoveToLocation(cell->GetLocation());
		boufbowl_player->PlayRunAnimation();
	}
}

void ABoufbowlPlayerController::LeftMouseButtonClick()
{
	UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayerController::LeftMouseButtonClick"));

	if (m_HitCursor.bBlockingHit)
	{
		UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayerController::LeftMouseButtonClick hit something"));

		if (m_BoufbowlGrid)
		{
			ABoufbowlCell* hit_cell = m_BoufbowlGrid->GetCellFromLocation(m_HitCursor.Location);

			if (hit_cell)
			{
				// Select a cell
				if (!m_SelectedCell)
				{
					SelectCell(hit_cell);
					SpawnPlayerSelectedCell();
				}
				// Move player to the hit cell
				else if (!hit_cell->GetBoufbowlPlayer())
				{
					MovePlayerToCell(hit_cell);
					hit_cell->SetBoufbowlPlayer(m_SelectedCell->GetBoufbowlPlayer());
					m_SelectedCell->SetBoufbowlPlayer(nullptr);
					DeselectCell();
					SelectCell(hit_cell);
				}	
				// Hit cell is occupied
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("ABoufbowlCell::LeftMouseButtonClick cell is occupied, won't move player"));
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ABoufbowlPlayerController::LeftMouseButtonClick m_BoufbowlGrid is nullptr"));
		}
	}
}

ABoufbowlCell* ABoufbowlPlayerController::GetSelectedCell()
{
	return m_SelectedCell;
}

void ABoufbowlPlayerController::SelectCell(ABoufbowlCell* cell_to_select)
{
	m_SelectedCell = cell_to_select;
	m_SelectedCell->CreateCellUI();

	UStaticMeshComponent* static_mesh_component = m_SelectedCell->FindComponentByClass<UStaticMeshComponent>();

	if (static_mesh_component)
	{
		static_mesh_component->SetMaterial(0, m_SelectionMaterial);
		UE_LOG(LogTemp, Log, TEXT("Changed material to selected"));
	}
}

void ABoufbowlPlayerController::DeselectCell()
{
	m_SelectedCell->DestroyCellUI();

	UStaticMeshComponent* static_mesh_component = m_SelectedCell->FindComponentByClass<UStaticMeshComponent>();

	if (static_mesh_component)
	{
		static_mesh_component->SetMaterial(0, m_BaseCellMaterial);
		UE_LOG(LogTemp, Log, TEXT("Changed material to non-selected"));
	}
	m_SelectedCell = nullptr;
}

void ABoufbowlPlayerController::RightMouseButtonClick()
{
	UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayerController::RightMouseButtonClick"));

	if (m_SelectedCell)
	{
		DeselectCell();
	}
}

void ABoufbowlPlayerController::SetGrid(ABoufbowlGrid* boufbowl_grid)
{
	m_BoufbowlGrid = boufbowl_grid;
}