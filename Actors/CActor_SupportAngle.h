

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CActor_SupportAngle.generated.h"

UCLASS()
class PROJECT_BASKETBALL_API ACActor_SupportAngle : public AActor
{
	GENERATED_BODY()
	
public:	

	ACActor_SupportAngle();

protected:

	virtual void BeginPlay() override;


private:

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* mesh;

};
