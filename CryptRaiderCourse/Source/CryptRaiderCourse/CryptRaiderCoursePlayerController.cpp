// Copyright Epic Games, Inc. All Rights Reserved.


#include "CryptRaiderCoursePlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "CryptRaiderCourseCameraManager.h"

ACryptRaiderCoursePlayerController::ACryptRaiderCoursePlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = ACryptRaiderCourseCameraManager::StaticClass();
}

void ACryptRaiderCoursePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
		{
			Subsystem->AddMappingContext(CurrentContext, 0);
		}
	}
}
