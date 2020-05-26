#include "CellUI.h"

#include "Components/Button.h"
#include "BoufbowlCell.h"

UCellUI::UCellUI(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UCellUI::SetCell(ABoufbowlCell* cell)
{
	m_BoufbowlCell = cell;
}

void UCellUI::OnClickSpawnButton()
{
	UE_LOG(LogTemp, Log, TEXT("UCellUI::OnClickSpawnButton"));
}

void UCellUI::OnClickMoveButton()
{
	UE_LOG(LogTemp, Log, TEXT("UCellUI::OnClickMoveButton"));
}

void UCellUI::OnClickAttackButton()
{
	UE_LOG(LogTemp, Log, TEXT("UCellUI::OnClickAttackButton"));
}

void UCellUI::OnClickBallButton()
{
	UE_LOG(LogTemp, Log, TEXT("UCellUI::OnClickBallButton"));
}

void UCellUI::NativeConstruct()
{
	// Do some custom setup

	// Call the Blueprint "Event Construct" node
	Super::NativeConstruct();

	if (!m_SpawnButton->OnClicked.IsBound())
		m_SpawnButton->OnClicked.AddDynamic(this, &UCellUI::OnClickSpawnButton);
	if (!m_MoveButton->OnClicked.IsBound())
		m_MoveButton->OnClicked.AddDynamic(this, &UCellUI::OnClickMoveButton);
	if (!m_AttackButton->OnClicked.IsBound())
		m_AttackButton->OnClicked.AddDynamic(this, &UCellUI::OnClickAttackButton);
	if (!m_BallButton->OnClicked.IsBound())
		m_BallButton->OnClicked.AddDynamic(this, &UCellUI::OnClickBallButton);
}


void UCellUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// Make sure to call the base class's NativeTick function
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Do your custom tick stuff here
}