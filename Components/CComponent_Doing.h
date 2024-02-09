
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CComponent_Doing.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_BASKETBALL_API UCComponent_Doing : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCComponent_Doing();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:

	void SpawnBasketball(class ACharacter* OwnerChar, float Inangle, float InSign , float InPower);


	void SpawnSupportAngle(class ACharacter* OwnerChar, FVector InLocation);


public:

	void SetSupportAngle_Location(FVector InLocation);
	FVector GetSupportAngle_Location();
	FVector GetSupportAngle_FoawrdVector();

private:

	//소환할 공 액터클래스
	TSubclassOf<class ACActor_Basketball>BP_Basketball;

	//앵글계산을 위한 액터
	TSubclassOf<class ACActor_SupportAngle>BP_SupAngle;
	

private:

	class ACActor_SupportAngle* SupAngle;
};
