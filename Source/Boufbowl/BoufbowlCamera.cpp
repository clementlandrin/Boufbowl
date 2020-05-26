#include "BoufbowlCamera.h"
#include "Components/InputComponent.h"
#include "Engine/GameViewportClient.h"
#include "Camera/CameraComponent.h"
#include "Engine/GameViewportClient.h"
#include "Engine.h"

ABoufbowlCamera::ABoufbowlCamera(const FObjectInitializer& ObjectInitializer)
{
	// enable Tick function
	PrimaryActorTick.bCanEverTick = true;

	// disable standard WASD movement
	bAddDefaultMovementBindings = false;

	// not needed Pitch Yaw Roll
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// set defaults

	CameraXYLimit = 25000.f;
	CameraHeight = 1000.f;
	CameraHeightMin = 0.0f;
	CameraHeightMax = 5000.f;

	CameraRadius = 2000.f;
	CameraRadiusMin = 1.0f;                // 100 for debugging
	CameraRadiusMax = 8000.f;

	CameraZAnlge = 0.f;                        // yaw

	CameraHeightAngle = 30.f;                // pitch
	CameraHeightAngleMin = 0.0f;
	CameraHeightAngleMax = 90.f;

	CameraZoomSpeed = 200.f;                // wheel
	CameraRotationSpeed = 10.f;                // wheel + ctrl
	CameraMovementSpeed = 3000.f;            // in all directions

	CameraScrollBoundary = 25.f;            // screen edge width

	bCanMoveCamera = true;

	// intialize the camera
	CameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("RTS Camera"));
	CameraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	CameraComponent->bUsePawnControlRotation = false;

	RepositionCamera();
}


//////////////////////////////////////////////////////////////////


void ABoufbowlCamera::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	if (!PlayerInputComponent) return;

	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// action mappings

	// mouse zoom
	PlayerInputComponent->BindAction("ZoomOutByWheel", IE_Pressed, this, &ABoufbowlCamera::ZoomOutByWheel);
	PlayerInputComponent->BindAction("ZoomInByWheel", IE_Pressed, this, &ABoufbowlCamera::ZoomInByWheel);

	PlayerInputComponent->BindAction("RotateUpByWheel", IE_Pressed, this, &ABoufbowlCamera::RotateUpByWheel);
	PlayerInputComponent->BindAction("RotateDownByWheel", IE_Pressed, this, &ABoufbowlCamera::RotateDownByWheel);

	// axis mappings

	// keyboard move (WASD, Home/End)
	PlayerInputComponent->BindAxis("MoveCameraForward", this, &ABoufbowlCamera::MoveCameraForwardInput);
	PlayerInputComponent->BindAxis("MoveCameraRight", this, &ABoufbowlCamera::MoveCameraRightInput);
	PlayerInputComponent->BindAxis("MoveCameraUp", this, &ABoufbowlCamera::MoveCameraUpInput);

	// double speed (WASD +Shift)
	PlayerInputComponent->BindAxis("FastMove", this, &ABoufbowlCamera::FastMoveInput);
	// yaw and pitch (WASD +Ctrl)
	PlayerInputComponent->BindAction("RotateLeftByWheel", IE_Pressed, this, &ABoufbowlCamera::RotateLeftByWheel);
	PlayerInputComponent->BindAction("RotateRightByWheel", IE_Pressed, this, &ABoufbowlCamera::RotateRightByWheel);
}

void ABoufbowlCamera::ZoomInByWheel()
{
	if (!bCanMoveCamera)    return;

	CameraRadius -= CameraZoomSpeed * FastMoveValue;
}


void ABoufbowlCamera::ZoomOutByWheel()
{
	if (!bCanMoveCamera)    return;

	CameraRadius += CameraZoomSpeed * FastMoveValue;
}


void ABoufbowlCamera::RotateLeftByWheel()
{
	if (!bCanMoveCamera)    return;

	CameraZAnlge -= CameraRotationSpeed * FastMoveValue;
}


void ABoufbowlCamera::RotateRightByWheel()
{
	if (!bCanMoveCamera)    return;

	CameraZAnlge += CameraRotationSpeed * FastMoveValue;
}


void ABoufbowlCamera::RotateUpByWheel()
{
	if (!bCanMoveCamera)    return;

	//CameraHeightAngle += CameraRotationSpeed * FastMoveValue;
	//CameraHeightAngle = FMath::Clamp(CameraHeightAngle, CameraHeightAngleMin, CameraHeightAngleMax);
}


void ABoufbowlCamera::RotateDownByWheel()
{
	if (!bCanMoveCamera)    return;

	//CameraHeightAngle -= CameraRotationSpeed * FastMoveValue;
	//CameraHeightAngle = FMath::Clamp(CameraHeightAngle, CameraHeightAngleMin, CameraHeightAngleMax);
}

//---------------

void ABoufbowlCamera::RepositionCamera()
{
	FVector NewLocation(0.f, 0.f, 0.f);
	FRotator NewRotation(0.f, 0.f, 0.f);

	float sinCameraZAngle = FMath::Sin(FMath::DegreesToRadians(CameraZAnlge));
	float cosCameraZAngle = FMath::Cos(FMath::DegreesToRadians(CameraZAnlge));

	float sinCameraHeightAngle = FMath::Sin(FMath::DegreesToRadians(CameraHeightAngle));
	float cosCameraHeightAngle = FMath::Cos(FMath::DegreesToRadians(CameraHeightAngle));

	NewLocation.X = cosCameraZAngle * cosCameraHeightAngle * CameraRadius;
	NewLocation.Y = sinCameraZAngle * cosCameraHeightAngle * CameraRadius;
	NewLocation.Z = sinCameraHeightAngle * CameraRadius;

	// new camera location
	CameraComponent->SetRelativeLocation(NewLocation);

	// new camera rotation
	NewRotation = (FVector(0.0f, 0.0f, 0.0f) - NewLocation).Rotation();
	CameraComponent->SetRelativeRotation(NewRotation);
}

void ABoufbowlCamera::FastMoveInput(float Direction)
{
	if (!bCanMoveCamera)    return;

	// left or right does not matter, to set double speed in any direction
	FastMoveValue = FMath::Abs(Direction) * 2.0f;

	// used as multiplier so must be 1 if not pressed
	if (FastMoveValue == 0.0f)
	{
		FastMoveValue = 1.0f;
	}
}


void ABoufbowlCamera::RotateInput(float Direction)
{
	if(Direction != 0.0)
		UE_LOG(LogTemp, Error, TEXT("Rotate"));
	if (!bCanMoveCamera)    return;

	// left or right does not matter
	RotateValue = FMath::Abs(Direction);
}


void ABoufbowlCamera::MoveCameraForwardInput(float Direction)
{
	if (!bCanMoveCamera)    return;

	MoveForwardValue = Direction;
}


void ABoufbowlCamera::MoveCameraRightInput(float Direction)
{
	if (!bCanMoveCamera)    return;

	MoveRightValue = Direction;
}


void ABoufbowlCamera::MoveCameraUpInput(float Direction)
{
	if (!bCanMoveCamera)    return;

	MoveUpValue = Direction;
}


void ABoufbowlCamera::ZoomCameraInInput(float Direction)
{
	if (!bCanMoveCamera)    return;

	ZoomInValue = Direction;
}

FVector ABoufbowlCamera::MoveCameraForward(float Direction)
{
	float MovementValue = Direction * CameraMovementSpeed;
	FVector DeltaMovement = MovementValue * GetIsolatedCameraYaw().Vector();
	return DeltaMovement;
}


FVector ABoufbowlCamera::MoveCameraRight(float Direction)
{
	float MovementValue = Direction * CameraMovementSpeed;
	FVector DeltaMovement = MovementValue * (FRotator(0.0f, 90.0f, 0.0f) + GetIsolatedCameraYaw()).Vector();
	return DeltaMovement;
}


FRotator ABoufbowlCamera::GetIsolatedCameraYaw()
{
	// FRotator containing Yaw only

	FTransform T = CameraComponent->GetComponentTransform();
	return FRotator(0.0f, T.Rotator().Yaw, 0.0f);
}

float ABoufbowlCamera::MoveCameraUp(float Direction)
{
	return Direction * CameraMovementSpeed;
}

void ABoufbowlCamera::ZoomCameraIn(float Direction)
{
	float MovementValue = Direction * CameraMovementSpeed;
	CameraRadius += MovementValue;
	CameraRadius = FMath::Clamp(CameraRadius, CameraRadiusMin, CameraRadiusMax);
}


void ABoufbowlCamera::TurnCameraUp(float Direction)
{
	//CameraHeightAngle -= Direction * CameraRotationSpeed * 10.0f;
	//CameraHeightAngle = FMath::Clamp(CameraHeightAngle, CameraHeightAngleMin, CameraHeightAngleMax);
}


void ABoufbowlCamera::TurnCameraRight(float Direction)
{
	CameraZAnlge += Direction * CameraRotationSpeed * 10.0f;
}

void ABoufbowlCamera::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// mouse position and screen size
	FVector2D MousePosition;
	FVector2D ViewportSize;

	UGameViewportClient* GameViewport = GEngine->GameViewport;

	if (!GameViewport) return;
	GameViewport->GetViewportSize(ViewportSize);

	// if viewport is focused, contains the mouse, and camera movement is allowed
	if (GameViewport->IsFocused(GameViewport->Viewport)
		&& GameViewport->GetMousePosition(MousePosition) && bCanMoveCamera)
	{
		//-------------------
		// movement direction by mouse at screen edge

		if (MousePosition.X < CameraScrollBoundary)
		{
			MoveRightValue = -1.0f;
		}
		else if (ViewportSize.X - MousePosition.X < CameraScrollBoundary)
		{
			MoveRightValue = 1.0f;
		}

		if (MousePosition.Y < CameraScrollBoundary)
		{
			MoveForwardValue = 1.0f;
		}
		else if (ViewportSize.Y - MousePosition.Y < CameraScrollBoundary)
		{
			MoveForwardValue = -1.0f;
		}

		// tweak camera actor position
		FVector ActualLocation = GetActorLocation();
		FVector ActualMovement = FVector::ZeroVector;

		// horizontal movement
		if (RotateValue == 0.f)
		{
			ActualMovement += MoveCameraForward(MoveForwardValue * FastMoveValue * DeltaSeconds);
			ActualMovement += MoveCameraRight(MoveRightValue * FastMoveValue * DeltaSeconds);
		}
		ActualLocation += ActualMovement;

		// vertical movement
		CameraHeight += MoveCameraUp(MoveUpValue * FastMoveValue * DeltaSeconds);
		CameraHeight = FMath::Clamp(CameraHeight, CameraHeightMin, CameraHeightMax);

		ActualLocation.Z = CameraHeight;

		// limit movement area
		ActualLocation.X = FMath::Clamp(ActualLocation.X, -CameraXYLimit, CameraXYLimit);
		ActualLocation.Y = FMath::Clamp(ActualLocation.Y, -CameraXYLimit, CameraXYLimit);

		// move actor
		SetActorLocation(ActualLocation);

		// tweak camera component relative transform
		// set rotation parameters
		/*if (RotateValue != 0.f)
		{
			TurnCameraUp(MoveForwardValue * FastMoveValue * DeltaSeconds);
			TurnCameraRight(MoveRightValue * FastMoveValue * DeltaSeconds);
		}*/

		// set zoom distance
		ZoomCameraIn(ZoomInValue * FastMoveValue * DeltaSeconds);

		// adjust camera component relative location and rotation
		RepositionCamera();
	}
}