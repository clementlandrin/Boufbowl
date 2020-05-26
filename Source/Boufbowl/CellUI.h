#pragma once

#include "Blueprint/UserWidget.h"
#include "CellUI.generated.h"

class UButton;
class ABoufbowlCell;

// We make the class abstract, as we don't want to create
// instances of this, instead we want to create instances
// of our UMG Blueprint subclass.
UCLASS(Abstract)
class UCellUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget)) UButton* m_SpawnButton;
	UPROPERTY(meta = (BindWidget)) UButton* m_MoveButton;
	UPROPERTY(meta = (BindWidget)) UButton* m_AttackButton;
	UPROPERTY(meta = (BindWidget)) UButton* m_BallButton;

	UFUNCTION() void OnClickSpawnButton();
	UFUNCTION() void OnClickMoveButton();
	UFUNCTION() void OnClickAttackButton();
	UFUNCTION() void OnClickBallButton();


	UCellUI(const FObjectInitializer& ObjectInitializer);

	// Optionally override the Blueprint "Event Construct" event
	virtual void NativeConstruct() override;

	// Optionally override the tick event
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void SetCell(ABoufbowlCell* cell);

private:
	ABoufbowlCell* m_BoufbowlCell;
};