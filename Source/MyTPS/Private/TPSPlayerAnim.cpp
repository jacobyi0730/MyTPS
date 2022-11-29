// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayerAnim.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void UTPSPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	auto owner = TryGetPawnOwner();
	ATPSPlayer* player = Cast<ATPSPlayer>(owner);
	if (player)
	{
		auto velocity = player->GetVelocity();
		auto forward = player->GetActorForwardVector();
		auto right = player->GetActorRightVector();

		forwardVelocity = FVector::DotProduct(velocity, forward);
		rightVelocity = FVector::DotProduct(velocity, right);
		
		UCharacterMovementComponent* movement = player->GetCharacterMovement();

		isInAir = movement->IsFalling();
	}
}