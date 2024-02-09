
#include "Components/CComponent_Doing.h"
#include "Actors/CActor_Basketball.h"
#include "Actors/CActor_SupportAngle.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Tools/CUsingTools.h"

UCComponent_Doing::UCComponent_Doing()
{

	//PrimaryComponentTick.bCanEverTick = true;

	//소환할 공 액터클래스
	ConstructorHelpers::FClassFinder<ACActor_Basketball>bs_class(TEXT("Blueprint'/Game/BP_CActor_Basketball_R.BP_CActor_Basketball_R_C'"));
	BP_Basketball = bs_class.Class;

	//앵글계산을 위한 액터
	ConstructorHelpers::FClassFinder<ACActor_SupportAngle>bs_class_sup(TEXT("Blueprint'/Game/Actors/BP_CActor_SupportAngle.BP_CActor_SupportAngle_C'"));
	BP_SupAngle = bs_class_sup.Class;

}


void UCComponent_Doing::BeginPlay()
{
	Super::BeginPlay();

}


void UCComponent_Doing::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
}

void UCComponent_Doing::SpawnBasketball(class ACharacter* OwnerChar, float Inangle, float InSign, float InPower)
{

	//위치 
	FTransform transform;
	transform.SetLocation(FVector(OwnerChar->GetActorLocation() + FVector(0.0f,0.0f,150.0f)));

	class ACActor_Basketball* b = OwnerChar->GetWorld()->SpawnActor<ACActor_Basketball>(BP_Basketball, transform);
	
	if (b != nullptr)
	{
		b->Set_power(InPower);
		b->Set_Angle(Inangle,InSign);
	}

}


void UCComponent_Doing::SpawnSupportAngle(class ACharacter* OwnerChar, FVector InLocation)
{
	FTransform transform;
	transform.SetLocation(InLocation);

	if (BP_SupAngle != nullptr)
	{
		SupAngle = OwnerChar->GetWorld()->SpawnActor<class ACActor_SupportAngle>
			(BP_SupAngle, transform);

	}

}


void UCComponent_Doing::SetSupportAngle_Location(FVector InLocation)
{
	if (SupAngle != nullptr)
		SupAngle->SetActorLocation(InLocation);

}

FVector UCComponent_Doing::GetSupportAngle_Location()
{
	return SupAngle->GetActorLocation();
}

FVector UCComponent_Doing::GetSupportAngle_FoawrdVector()
{
	return  SupAngle->GetActorForwardVector();
}
