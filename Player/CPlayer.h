
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CPlayer.generated.h"

UCLASS()
class PROJECT_BASKETBALL_API ACPlayer : public ACharacter
{
	GENERATED_BODY()

public:

	ACPlayer();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:


	UPROPERTY(VisibleDefaultsOnly)
		class USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly)
		class UCameraComponent* Camera;


	//À§Á¬
	TSubclassOf<class UCUserWidget_Gauge> GaugeBarClass;
	class UCUserWidget_Gauge* Bar;

private:

	//ÀÌµ¿

	void MoveForward(float input);
	void MoveRight(float input);

	void Sprint_On();
	void Sprint_Off();
	//¸¶¿ì½º½Ã¾ß
	void MoveVerticalLook(float input);
	void MoveHorizontalLook(float input);
	

private:

	//½¸
	void Shut_Pressed();
	void Shut_Released();

private:

	void Failed_My_Think();

private:

	UPROPERTY(EditDefaultsOnly, Category = "CameraSpeed")
		float HorizontalLook = 45.0f;

	UPROPERTY(EditDefaultsOnly, Category = "CameraSpeed")
		float VerticalLook = 45.0f;


public:

	FORCEINLINE bool IsSprinting() { return bSprint; }

private:

	bool bSprint = false;

	class UCComponent_Doing* doingComp;


private:

	class APlayerCameraManager* playerCameraManager;
	UPROPERTY(EditAnywhere, Category = "LineTraceDistance")
		float lineDistance = 1000.0f;

	TArray<class AActor*>IgnoreActors;
	FHitResult Hitresult;
	FHitResult Hitresult_Land;


private:
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes_Rand;



private:

	float Degree = 0.0f;
	float SignDot = 0.0f;
	float Power = 0.0f;
	bool bPressed = false;
	bool bSucceedTrace = false;

	FVector SupportActor_Origin_Location = FVector(9999.0f, 99990.0f, 9999.9f);


private:

	UPROPERTY(EditDefaultsOnly, Category = "MaxGauge")
		float MaxGauge = 80.0f;
};
