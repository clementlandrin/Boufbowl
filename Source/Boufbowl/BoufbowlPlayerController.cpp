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

	ConstructorHelpers::FObjectFinder<UMaterial> SavedMaterialAsset(TEXT("Material'/Game/Geometry/Meshes/SavedMaterial.SavedMaterial'"));
	if (SavedMaterialAsset.Succeeded())
	{
		m_SavedMaterial = SavedMaterialAsset.Object;
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

void ABoufbowlPlayerController::MoveSavedPlayerToSelectedCell()
{
	if (m_SavedCell && m_SavedCell->GetBoufbowlPlayer() && !m_SelectedCell->GetBoufbowlPlayer())
	{
		UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayerController::MovePlayerToCell moving player to destination cell"));
		ABoufbowlPlayer* boufbowl_player = m_SavedCell->GetBoufbowlPlayer();
		AAIController* ai_controller = boufbowl_player->GetAIController();
		ai_controller->MoveToLocation(m_SelectedCell->GetLocation());
		//boufbowl_player->PlayRunAnimation();

		m_SelectedCell->SetBoufbowlPlayer(boufbowl_player);
		UnsaveCell();
		DeselectCell();
	}
}

void ABoufbowlPlayerController::AttackEnnemyWithSavedPlayer()
{

	if (m_SavedCell && m_SavedCell->GetBoufbowlPlayer() && m_SelectedCell->GetBoufbowlPlayer())
	{
		if (!m_SelectedCell->GetBoufbowlPlayer()->GetOwnerController() || m_SelectedCell->GetBoufbowlPlayer()->GetOwnerController() != this)
		UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayerController::AttackEnnemyWithSavedPlayer attacking ennemy"));
		ABoufbowlPlayer* ennemy_player = m_SelectedCell->GetBoufbowlPlayer();
		
		ennemy_player->TakeDamage(1);
		UnsaveCell();
		DeselectCell();
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
					UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayerController::LeftMouseButtonClick no selected cell"));
					if (!hit_cell->GetBoufbowlPlayer())
					{
						UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayerController::LeftMouseButtonClick hitten cell has no player, won't select"));
						return;
					}

					UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayerController::LeftMouseButtonClick hitten cell has a player, select"));
					SelectCell(hit_cell);
				}
				// if one cell is selected and the player is ours, save the selected cell and selected the hitten one
				else if (m_SelectedCell->GetBoufbowlPlayer() && m_SelectedCell->GetBoufbowlPlayer()->GetOwnerController() && (m_SelectedCell->GetBoufbowlPlayer()->GetOwnerController() == this))
				{
					UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayerController::LeftMouseButtonClick selected player is ours, saving cell process"));
					ABoufbowlCell* temp_cell = m_SelectedCell;
					DeselectCell();
					SaveCell(temp_cell);
					SelectCell(hit_cell);
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayerController::LeftMouseButtonClick there is a selected cell but player is not ours"));
					DeselectCell();

					if (hit_cell->GetBoufbowlPlayer())
					{
						UE_LOG(LogTemp, Log, TEXT("ABoufbowlPlayerController::LeftMouseButtonClick hitten cell has a player, select"));
						SelectCell(hit_cell);
					}
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ABoufbowlPlayerController::LeftMouseButtonClick m_BoufbowlGrid is nullptr"));
		}
	}
}

void ABoufbowlPlayerController::SaveCell(ABoufbowlCell* cell_to_save)
{
	m_SavedCell = cell_to_save;

	UStaticMeshComponent* static_mesh_component = m_SavedCell->FindComponentByClass<UStaticMeshComponent>();

	if (static_mesh_component)
	{
		static_mesh_component->SetMaterial(0, m_SavedMaterial);
		UE_LOG(LogTemp, Log, TEXT("Changed material to saved"));
	}
}

void ABoufbowlPlayerController::UnsaveCell()
{
	UStaticMeshComponent* static_mesh_component = m_SavedCell->FindComponentByClass<UStaticMeshComponent>();

	if (static_mesh_component)
	{
		static_mesh_component->SetMaterial(0, m_BaseCellMaterial);
		UE_LOG(LogTemp, Log, TEXT("Changed material to base"));
	}

	m_SavedCell = nullptr;
}

ABoufbowlCell* ABoufbowlPlayerController::GetSelectedCell()
{
	return m_SelectedCell;
}

ABoufbowlCell* ABoufbowlPlayerController::GetSavedCell()
{
	return m_SavedCell;
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
	if (!m_SelectedCell)
		return;

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
	if (m_SavedCell)
	{
		UnsaveCell();
	}
}

void ABoufbowlPlayerController::SetGrid(ABoufbowlGrid* boufbowl_grid)
{
	m_BoufbowlGrid = boufbowl_grid;
}