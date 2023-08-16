// Copyright Epic Games, Inc. All Rights Reserved.

#include "AudioGameGameMode.h"
#include "AudioGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAudioGameGameMode::AAudioGameGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
