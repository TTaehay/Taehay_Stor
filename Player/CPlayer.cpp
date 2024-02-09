
#include "Player/CPlayer.h"
#include "Tools/CUsingTools.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include <Kismet/KismetSystemLibrary.h>
#include "Components/CComponent_Doing.h"
#include "Widget/CUserWidget_Gauge.h"
ACPlayer::ACPlayer()
{
 
	PrimaryActorTick.bCanEverTick = true;

	//스프링암 & 카메라
	SpringArm = this->CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera = this->CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);

	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	SpringArm->TargetArmLength = 350.0f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;
	Camera->bUsePawnControlRotation = false;
	Camera->SetRelativeRotation(FRotator(-20.0f, 0.0f, 0.0f));
	
	//메쉬
	USkeletalMesh* mesh;
	ConstructorHelpers::FObjectFinder<USkeletalMesh>asset(TEXT("SkeletalMesh'/Game/locomotiom/Mesh/Ch36_nonPBR.Ch36_nonPBR'"));
	mesh = asset.Object;
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -80.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));

	//애님인스턴스
	TSubclassOf<UAnimInstance>animIns;
	ConstructorHelpers::FClassFinder<UAnimInstance>ains(TEXT("AnimBlueprint'/Game/BP_CAmimIns.BP_CAmimIns_C'"));
	animIns = ains.Class;
	GetMesh()->SetAnimInstanceClass(animIns);

	GetCharacterMovement()->MaxWalkSpeed = 200.0f;


	//농구공 소환 액터 컴포넌트
	doingComp = this->CreateDefaultSubobject<UCComponent_Doing>("DoingComp");
	

	//위젯
	ConstructorHelpers::FClassFinder<UCUserWidget_Gauge>c(TEXT("WidgetBlueprint'/Game/Widget/Widget_BP.Widget_BP_C'"));
	GaugeBarClass = c.Class;

	IgnoreActors.Add(this);
}

void ACPlayer::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* controller = this->GetController<APlayerController>();

	//게이지바 Viewport
	if (!!controller)
	{
		Bar = CreateWidget<UCUserWidget_Gauge, APlayerController>(GetController<APlayerController>(), GaugeBarClass);
		
		Bar->AddToViewport();
	}
	
	//오브젝트 타입 넣기
	{
		TEnumAsByte<EObjectTypeQuery> trace = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2);

		ObjectTypes.Add(trace);

		TEnumAsByte<EObjectTypeQuery> land_object = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3);

		ObjectTypes_Rand.Add(land_object);
	}

	//두번째 트레이스 액터스폰
	doingComp->SpawnSupportAngle(this, SupportActor_Origin_Location);
		
}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//지점 트레이스
	UKismetSystemLibrary::LineTraceSingleForObjects(this,
		this->GetActorLocation() + FVector(0.0f, 0.0f, 30),
		{
			   (this->GetController()->GetControlRotation().Vector() * lineDistance) + this->GetActorLocation() + FVector(0.0f, 0.0f, 30)
		},
		ObjectTypes,
		false,
		IgnoreActors,
		EDrawDebugTrace::ForOneFrame,
		Hitresult,
		true
	);
	
	//트레이스 성공했을때
	if (Hitresult.Actor != nullptr)
	{
		//트레이스 성공 bool 
		bSucceedTrace = true;
		//충돌한 위치에서 아래 위치 기준 설정
		{
			CUsingTools::TraceSingleObject
			(
				this,
				Hitresult.ImpactPoint,
				Hitresult.ImpactPoint - FVector(0.0f, 0.0f, 1000.0f),
				ObjectTypes_Rand,
				false,
				IgnoreActors,
				EDrawDebugTrace::ForOneFrame,
				Hitresult_Land,
				true
			);

		}

		//바닥지점에서 방향벡터를 위한 엑터 소환
		if (Hitresult_Land.bBlockingHit)
		{
			doingComp->SetSupportAngle_Location(Hitresult_Land.ImpactPoint);
		
			//내적
			FVector Center_Foraward = doingComp->GetSupportAngle_FoawrdVector();

			FVector ToTarget_Direction = doingComp->GetSupportAngle_Location() - this->GetActorLocation();

			ToTarget_Direction.Z = 0;
			ToTarget_Direction.Normalize();

			float InNerProduct = FVector::DotProduct(Center_Foraward, ToTarget_Direction);
			Degree = UKismetMathLibrary::DegAcos(InNerProduct);

			//CUsingTools::Print_string(GetWorld(), FString::SanitizeFloat(Degree));

			//외적으로 왼편 오른편
			FVector OutProduct = FVector::CrossProduct(Center_Foraward, ToTarget_Direction);
			SignDot = UKismetMathLibrary::SignOfFloat(OutProduct.Z);
		
		}

	}
	else
	{
		bSucceedTrace = false;
		doingComp->SetSupportAngle_Location(SupportActor_Origin_Location);
	}

	//Gauge Condition
	if (bPressed)
	{
		Power += 1.0f;
		Power = UKismetMathLibrary::Min(Power, MaxGauge);
		Bar->UpdateGauge(MaxGauge, Power);
		return;
	}

	Power -= 1.0f;
	Power = UKismetMathLibrary::Max(Power, 0.0f);
	Bar->UpdateGauge(MaxGauge, Power);
	


}

void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward",this, &ACPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight",this, &ACPlayer::MoveRight);
	PlayerInputComponent->BindAxis("VerticalLook", this, &ACPlayer::MoveVerticalLook);
	PlayerInputComponent->BindAxis("HorizontalLook", this, &ACPlayer::MoveHorizontalLook);

	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, this, &ACPlayer::Sprint_On);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, this, &ACPlayer::Sprint_Off);

	PlayerInputComponent->BindAction("Shut", EInputEvent::IE_Pressed, this, &ACPlayer::Shut_Pressed);
	PlayerInputComponent->BindAction("Shut", EInputEvent::IE_Released, this, &ACPlayer::Shut_Released);

}

void ACPlayer::MoveForward(float input)
{

	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	AddMovementInput(direction, input);
}

void ACPlayer::MoveRight(float input)
{
	FRotator rotator = FRotator(0, GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	AddMovementInput(direction, input);
}

void ACPlayer::Sprint_On()
{
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;
}
void ACPlayer::Sprint_Off()
{
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
}
void ACPlayer::MoveVerticalLook(float input)
{
	AddControllerPitchInput(input * VerticalLook * GetWorld()->GetWorld()->GetDeltaSeconds());
}

void ACPlayer::MoveHorizontalLook(float input)
{
	AddControllerYawInput(input * HorizontalLook * GetWorld()->GetWorld()->GetDeltaSeconds());
}

void ACPlayer::Shut_Pressed()
{
	bPressed = true;
}

void ACPlayer::Shut_Released()
{
	//백보드에 트레이스 될 때만
	if (!bSucceedTrace)
	{
		bPressed = false;
		return;
	}

	doingComp->SpawnBasketball(this, Degree, SignDot , Power + 80.0f);
	Power = 0.0001f;
	bPressed = false;
}

void ACPlayer::Failed_My_Think()
{
	//FVector P_direction = this->GetActorLocation().ForwardVector;



	//double test = asin
	//(
	//	((P_direction.X * Direction_hit.Z) - (Direction_hit.Z * P_direction.X))
	//	/
	//	(sqrt(pow(P_direction.X, 2) + pow(P_direction.Z, 2)) * sqrt(pow(Direction_hit.X, 2) +
	//		pow(Direction_hit.Z, 2)))
	//);
	//CUsingTools::Print_string(this->GetWorld(), FString::SanitizeFloat(test));

	//doingComp->SpawnBasketball(this, test);

	/*double a = (P_direction.X * Direction_hit.X) + (P_direction.Y * Direction_hit.Y) + (P_direction.Z * Direction_hit.Z);
	double b = sqrt(pow(P_direction.X, 2) + pow(P_direction.Y, 2) + pow(P_direction.Z, 2));
	double c = sqrt(pow(Direction_hit.X, 2) + pow(Direction_hit.Y, 2) + pow(Direction_hit.Z, 2));


	CUsingTools::Print_string(this->GetWorld(), "A : " + FString::SanitizeFloat(a));
	CUsingTools::Print_string(this->GetWorld(), "B : " + FString::SanitizeFloat(b));
	CUsingTools::Print_string(this->GetWorld(), "C : " + FString::SanitizeFloat(c));


	double angle = acos
	(
		((P_direction.X * Direction_hit.X) + (P_direction.Y * Direction_hit.Y) + (P_direction.Z * Direction_hit.Z))/

	   (
			sqrt(pow(P_direction.X, 2) + pow(P_direction.Y, 2) + pow(P_direction.Z, 2))
			*
			sqrt(pow(Direction_hit.X, 2) + pow(Direction_hit.Y, 2) + pow(Direction_hit.Z, 2))

		)

	);
	angle *= 100.0f;
	CUsingTools::Print_string(this->GetWorld(), FString::SanitizeFloat(angle));*/


	//doingComp->SpawnBasketball(this, angle);

		/*UKismetSystemLibrary::LineTraceSingle
	(
		this,
		this->GetActorLocation() + FVector(0.0f, 0.0f, 30),
		{
				GetActorLocation() + FVector
				(
					cos(SpringArm->GetTargetRotation().Roll * (3.141592 / 180)) * lineDistance,
					tan(SpringArm->GetTargetRotation().Yaw * (3.141592 / 180)) * lineDistance,
					sin(SpringArm->GetTargetRotation().Pitch * (3.141592 / 180)) * lineDistance
				)
		}
		,
		ETraceTypeQuery::TraceTypeQuery3,
		false,
		IgnoreActors,
		EDrawDebugTrace::ForOneFrame,
		Hitresult,
		true
	);*/

	//UKismetSystemLibrary::LineTraceSingleByProfile
	//(
	//	this,
	//	this->GetActorLocation() + FVector(0.0f, 0.0f, 30),
	//	{


	//		   (this->GetController()->GetControlRotation().Vector() * lineDistance) + this->GetActorLocation() + FVector(0.0f, 0.0f, 30)
	//			//GetActorLocation() + FVector
	//			//(
	//			//	cos(SpringArm->GetTargetRotation().Roll * (3.141592 / 180)) * lineDistance,
	//			//	tan(SpringArm->GetTargetRotation().Yaw * (3.141592 / 180)) * lineDistance,
	//			//	sin(SpringArm->GetTargetRotation().Pitch * (3.141592 / 180)) * lineDistance
	//			//)
	//	},
	//	FName(TEXT("Basketball_Object")),
	//	false,
	//	IgnoreActors,
	//	EDrawDebugTrace::ForOneFrame,
	//	Hitresult,
	//	true

	//);
}