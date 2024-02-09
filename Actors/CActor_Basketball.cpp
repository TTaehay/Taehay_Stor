
#include "Actors/CActor_Basketball.h"
#include "Components/SphereComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Tools/CUsingTools.h>
ACActor_Basketball::ACActor_Basketball()
{

	PrimaryActorTick.bCanEverTick = true;

	//충돌체 설정
	SphComp = this->CreateDefaultSubobject<USphereComponent>("Collision");

	//메쉬설정

	mesh = this->CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	ConstructorHelpers::FObjectFinder<UStaticMesh>asset(TEXT("StaticMesh'/Game/locomotiom/ball/basketball_dl.basketball_dl'"));
	mesh->SetStaticMesh(asset.Object);
	mesh->SetupAttachment(SphComp);

	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;


	
}


void ACActor_Basketball::BeginPlay()
{
	Super::BeginPlay();


	SphComp->OnComponentBeginOverlap.AddDynamic(this, &ACActor_Basketball::OnOverlapBegin);

	OriginalPos = this->GetActorLocation();

}


void ACActor_Basketball::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	//바닥충돌확인
	if ((GetActorLocation().Z - SphComp->GetScaledSphereRadius() <= 21.0f && bBound == false)|| v0 <= 40.0f)
	{
		Bound_Ground_Fix();
		//CUsingTools::Print_string(GetWorld(), FString::SanitizeFloat(v0));
		bBound = true;
	}
	else
	{
		Bouncing();
		time += DeltaTime * ballSpeed;
		if (GetActorLocation().Z - SphComp->GetScaledSphereRadius() >= 20.f + SphComp->GetScaledSphereRadius())
			bBound = false;
	}

}

void ACActor_Basketball::Bouncing()
{
	z = tan(radiant) * time
		- (
			gravity
			/
			(2.0f * v0 * v0 * cos(radiant) * cos(radiant)
			)) * time * time
		;

	
	this->SetActorLocation
	(
		FVector
		(
			OriginalPos.X += AddXpos * (1.0f - (Sup_angle / MaxAngle)) * overlapBound,
			OriginalPos.Y += AddYpos * Sup_sign * (Sup_angle / MaxAngle) ,
		    OriginalPos.Z + z
		)
	);
	
}

void ACActor_Basketball::OnLand(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	UKismetSystemLibrary::PrintString(GetWorld(), "Hit", true, true, FLinearColor::Blue);
	
}
void ACActor_Basketball::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//if(OtherActor && (OtherActor != this) && OtherComp)
	//	UKismetSystemLibrary::PrintString(GetWorld(), "Overlap", true, true, FLinearColor::Blue);

	overlapBound *= -1;
}
void ACActor_Basketball::Set_Angle(float Inangle , float InSign)
{
	Sup_angle = Inangle;

	Sup_sign = InSign;
}

void ACActor_Basketball::Set_power(float InPower)
{
	v0 = InPower;
}


void ACActor_Basketball::Bound_Ground_Fix()
{
	OriginalPos = GetActorLocation();
	time = 0.0f;
	z = 0.0f;
	v0 *= 0.70f;
}