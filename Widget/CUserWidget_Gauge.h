

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Gauge.generated.h"

UCLASS()
class PROJECT_BASKETBALL_API UCUserWidget_Gauge : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Gauge")
		void UpdateGauge(float MaxGauge, float InGauge);
		
	
};
