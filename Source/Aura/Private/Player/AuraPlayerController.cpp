// Fill out your copyright notice in the Description page of Project Settings.
#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext, 0);

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData; //used to configure cursor settings
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock); //we will not lock out mouse to the viewport
	InputModeData.SetHideCursorDuringCapture(false); // as soon as our cursor is captured into the viewport, it wont be hidden
	SetInputMode(InputModeData);
}
