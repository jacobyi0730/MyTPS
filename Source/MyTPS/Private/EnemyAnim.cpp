// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnim.h"
#include <Kismet/GameplayStatics.h>
#include "TPSPlayer.h"
void UEnemyAnim::AnimNotify_OnMyHit()
{
	// 플레이어와 나의 거리를 측정해서
	ATPSPlayer* player = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	AActor* enemy = TryGetPawnOwner();
	// attackDistance보다 작으면
	if (FVector::Distance(player->GetActorLocation(), enemy->GetActorLocation()) < attackDistance)
	{
		// 플레이어의 OnMyHitFromEnemy함수를 호출하고싶다.
		player->OnMyHitFromEnemy(1);
	}
}
