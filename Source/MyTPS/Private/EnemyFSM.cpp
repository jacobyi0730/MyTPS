// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include "TPSPlayer.h"
#include "MyTPS.h"
#include <Components/CapsuleComponent.h>

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
	me->AddMovementInput(dir.GetSafeNormal());
	// 만약 타겟과의 거리가 공격유효거리 보다 작다면
	float distance = dir.Size(); // FVector::Distance(me->GetActorLocation(), target->GetActorLocation()); // me->GetDistanceTo(target); // dir.Length();
	if (distance < attackDistance)
	{
		// 공격 상태로 전이하고싶다.
		state = EEnemyState::Attack;
	}
}

void UEnemyFSM::TickAttack()
{
	currentTime += GetWorld()->GetDeltaSeconds();	// 시간이 흐르다가
	if (currentTime > 1)	// 1초가 지나면 
	{
		PRINT_LOG(TEXT("Attack!!!"));// 공격을 하고
		// 플레이어와의 거리를 구하고
		float distance = (target->GetActorLocation() - me->GetActorLocation()).Size();
		// 거리가 공격유효거리보다 멀어졌다면
		if (distance >= attackDistance) {

			state = EEnemyState::Move;// 이동상태로 전이하고싶다.
		}
		currentTime = 0;// 현재시간을 초기화 하고싶다.
	}

}

void UEnemyFSM::TickDamage()
{
	// 0.5초 시간이 흐른 후 Move상태로 전이하고싶다.
	currentTime += GetWorld()->GetDeltaSeconds();
	if (currentTime > 0.5f)
	{
		state = EEnemyState::Move;
	}
}
void UEnemyFSM::TickDie()
{
	// 2초동안 아래로 이동 후 
	currentTime += GetWorld()->GetDeltaSeconds();

	// 아래로 이동처리
	me->SetActorLocation(me->GetActorLocation() + FVector::DownVector * 200 * GetWorld()->GetDeltaSeconds());

	if (currentTime > 2) {
		me->Destroy();
	}
}

void UEnemyFSM::SetStateDamage()
{
	state = EEnemyState::Damage;
}

void UEnemyFSM::SetStateDie()
{
	state = EEnemyState::Die;
	currentTime = 0;
	// 충돌체를 끄고싶다.
	auto col = me->GetCapsuleComponent();
	col->SetCollisionProfileName(TEXT("NoCollision"));
}
