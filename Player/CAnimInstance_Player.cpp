

#include "Player/CAnimInstance_Player.h"
#include "GameFramework/Character.h"
#include "Player/CPlayer.h"
#include "GameFramework/PlayerController.h"

void UCAnimInstance_Player::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
}

void UCAnimInstance_Player::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);


	if (OwnerCharacter == NULL)
		return;

	Direction = OwnerCharacter->GetVelocity().Size2D();
	Speed = CalculateDirection(OwnerCharacter->GetVelocity(), OwnerCharacter->GetControlRotation());

}
