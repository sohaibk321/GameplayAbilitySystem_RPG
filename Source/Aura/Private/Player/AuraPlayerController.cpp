// Fill out your copyright notice in the Description page of Project Settings.
#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::PlayerTick(float DeltaSeconds)
{
	Super::PlayerTick(DeltaSeconds);
	
	CursorTrace();
	
}

void AAuraPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);\
	if (!CursorHit.bBlockingHit) return;
	LastActor = ThisActor;
	ThisActor = CursorHit.GetActor();

	/**
	 * Line trace from cursor. There are several scenarios
	 * A. Last Actor is null && This Actor is null
	 *		- Do nothing
	 * B. Last Actor is null && ThisActor ils valid
	 *		- Call highlight on ThisActor since it's the first actor we hovered on
	 * C. LastActor is valid && ThisActor is null
	 *		- Unhighlight LastActor
	 * D. Both actors are valid but LastActor != ThisActor
	 *		- UnHighlight LastActor and Highlight ThisActor
	 * E. Both actors are valid and are the same actor
	 *		- Do nothing
	 */
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{// Case B
			ThisActor->HighlightActor();
		}
		else
		{
			// Case A: both are null, do nothing
		}
	}
	else // LastActor is valid
	{
		if (ThisActor == nullptr)
		{
			// Case C
			LastActor->UnHighlightActor();
		}
		else // Both actors are valid
		{
			if (LastActor != ThisActor)
			{
				// Case D
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
			else
			{
				// Case E - do nothing
			}
		}
	}
}
	
	
void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);//adds map context to player controller so it can receive data

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData; //used to configure cursor settings
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); //we will not lock out mouse to the viewport
	InputModeData.SetHideCursorDuringCapture(false); // as soon as our cursor is captured into the viewport, it won't be hidden
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2d InputAxisVector = InputActionValue.Get<FVector2d>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.0f);

	//the first part is used to get the camera angle as a reference point to figure out forward and sideways direction
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y); //first part is direction and 2nd part is scalar
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}
