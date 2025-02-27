// Copyright Epic Games, Inc. All Rights Reserved.

#include "CPPDemo_Ver55XGameMode.h"
#include "CPPDemo_Ver55XCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACPPDemo_Ver55XGameMode::ACPPDemo_Ver55XGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
