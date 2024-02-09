
#pragma once

#include "CoreMinimal.h"
#include <Kismet/KismetSystemLibrary.h>
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
class PROJECT_BASKETBALL_API CUsingTools
{
public:

	
	static void Print_string(UWorld* InWorld, FString InString,bool Something1= true, bool Something2 = true, FLinearColor InColor = FLinearColor::Blue)
	{
		UKismetSystemLibrary::PrintString(InWorld, InString, Something1, Something2, InColor);
	}

	static void Print_Vector(UWorld* InWorld, FVector InVector, bool Something1 = true, bool log = true, FLinearColor InColor = FLinearColor::Red)
	{
		UKismetSystemLibrary::PrintString(InWorld," X : " + FString::SanitizeFloat(InVector.X) + " Y : " + FString::SanitizeFloat(InVector.Y) + " Z : " + FString::SanitizeFloat(InVector.Z), Something1, log, InColor);
	
	}

	static float Max_float(float InA, float InB)
	{
		if (InA > InB)
			return InA;

		return InB;
	}

	static void TraceSingleObject(UObject* InObject,FVector InStart, FVector InEnd,
		TArray<TEnumAsByte<EObjectTypeQuery>>InObjectType,bool bTraceComplex,TArray<AActor*> IgnoreActors,EDrawDebugTrace::Type InDrawType,FHitResult& InResult, bool InIgnoreSelf)
	{
		UKismetSystemLibrary::LineTraceSingleForObjects
		(
			InObject,
			InStart,
			InEnd,
			InObjectType,
			bTraceComplex,
			IgnoreActors,
			InDrawType,
			InResult,
			InIgnoreSelf
		);
	}


};
