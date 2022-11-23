// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include "TPSPlayer.h"

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();
	state = EEnemyState::Idle;

	me = Cast<AEnemy>(GetOwner());
}




// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (state)
	{
	case EEnemyState::Idle:		TickIdle();		break;
	case EEnemyState::Move:		TickMove();		break;
	case EEnemyState::Attack:	TickAttack();	break;
	case EEnemyState::Damage:	TickDamage();	break;
	case EEnemyState::Die:		TickDie();		break;
	}
}

void UEnemyFSM::TickIdle()
{
	// 플레이어를 찾아서 목적지로 기억하고싶다.
	target = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	// 만약 찾았다면 
	if (target)
	{
		// 이동상태로 전이하고싶다.
		state = EEnemyState::Move;
	}
}

void UEnemyFSM::TickMove()
{
	// 타겟을 항해서 이동하고싶다.
	// P = P0 + vt
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	dir.Normalize();

	me->AddMovementInput(dir);
	// 만약 타겟과의 거리가 공격유효거리 보다 작다면
	// 공격 상태로 전이하고싶다.
}

void UEnemyFSM::TickAttack()
{
	
}

void UEnemyFSM::TickDamage()
{
	
}

void UEnemyFSM::TickDie()
{
	
}