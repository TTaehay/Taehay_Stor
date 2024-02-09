// Copyright Epic Games, Inc. All Rights Reserved.


#include "Project_basketballGameModeBase.h"

AProject_basketballGameModeBase::AProject_basketballGameModeBase()
{
	ConstructorHelpers::FClassFinder<APawn>playerclass(TEXT("Blueprint'/Game/BP_CPlayer.BP_CPlayer_C'"));

	DefaultPawnClass = playerclass.Class;

}