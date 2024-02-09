
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CActor_Basketball.generated.h"

DECLARE_MULTICAST_DELEGATE(FDel_Bound);

UCLASS()
class PROJECT_BASKETBALL_API ACActor_Basketball : public AActor
{
	GENERATED_BODY()
	
public:	

	ACActor_Basketball();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;


public:

	void Bouncing();

	void Set_Angle(float Inangle, float InSign);
	void Set_power(float InPower);

private:
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* mesh;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent* SphComp;

private:
	
	UFUNCTION()
	void OnLand(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


private:

	void Bound_Ground_Fix();

private:

	//
	float gravity = 30.0f;
	double z = 0.0f;
	double angle = 85.0f;
	double radiant = angle * 3.141519 / 180;

	UPROPERTY(EditAnywhere, Category = "InitSpeed")
	    float v0 = 100.0f;
	UPROPERTY(EditAnywhere, Category = "InitSpeed")
		float ballSpeed = 10.0f;
	UPROPERTY(EditAnywhere, Category = "InitSpeed")
		float AddXpos = 10.0f;
	UPROPERTY(EditAnywhere, Category = "InitSpeed")
		float AddYpos = 10.0f;

	FVector OriginalPos = FVector(0.0f, 0.0f, 0.0f);
	float time = 0.0f;
	float Sup_angle;

private:

	UPROPERTY(EditAnywhere, Category = "MaxAngle")
	   float MaxAngle = 88.0f;
	float Sup_sign = 0.0f;
	bool bBound = false;


private:

	int overlapBound = 1;

};
