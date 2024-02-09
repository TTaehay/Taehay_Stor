

#include "Actors/CActor_SupportAngle.h"

ACActor_SupportAngle::ACActor_SupportAngle()
{

	//메쉬설정
	mesh = this->CreateDefaultSubobject<UStaticMeshComponent>("Mesh");

	ConstructorHelpers::FObjectFinder<UStaticMesh>asset(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	mesh->SetStaticMesh(asset.Object);

	mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mesh->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;


	
}

void ACActor_SupportAngle::BeginPlay()
{
	Super::BeginPlay();
	
}


