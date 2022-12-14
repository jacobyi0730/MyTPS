// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFSM.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include "TPSPlayer.h"
#include "MyTPS.h"
#include <Components/CapsuleComponent.h>
#include "EnemyAnim.h"
#include <AIModule/Classes/AIController.h>
#include <NavigationSystem.h>
#include <AIModule/Classes/Navigation/PathFollowingComponent.h>

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

	// 태어날 때 EnemyAnim를 가져와서 enemyAnim변수에 넣어놓고싶다.
	enemyAnim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());

	ai = Cast<AAIController>(me->GetController());

	UpdateRandomDestination(me->GetActorLocation(), 500, destination);
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
	// 살아가면서 나의 state를 EnemyAnim의 enemyState에 값을 넣어주고싶다.

	//enemyAnim->enemyState = state;
}

void UEnemyFSM::TickIdle()
{
	// 플레이어를 찾아서 목적지로 기억하고싶다.
	target = Cast<ATPSPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	// 만약 찾았다면 
	if (target)	// 조건(Condition)
	{
		// 이동상태로 전이(Transition)하고싶다.
		SetState(EEnemyState::Move);
	}
}

void UEnemyFSM::TickMove()
{
	FVector dir = target->GetActorLocation() - me->GetActorLocation();
	float distance = dir.Size();

	// 목적지를 향해 이동하고싶다.
	// 네비게이션 시스템
	auto naviSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FPathFindingQuery query;
	FAIMoveRequest request;
	request.SetAcceptanceRadius(3);
	request.SetGoalLocation(target->GetActorLocation());
	ai->BuildPathfindingQuery(request, query);
	FPathFindingResult result = naviSystem->FindPathSync(query);
	// 검색 범위안에 플레이어가 있다. 플레이어의 위치로 이동하겠다.
	if (result.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation(target->GetActorLocation());
	}
	// 검색 범위안에 플레이어가 없으니 랜덤한 위치로 이동하겠다.
	else
	{
		auto r = ai->MoveToLocation(destination);
		// 만약 목적지에 도착했다면?
		if (r == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			// 목적지를 랜덤으로 갱신하고싶다.
			UpdateRandomDestination(me->GetActorLocation(), 500, destination);
		}
	}

	// 만약 타겟과의 거리가 공격유효거리 보다 작다면
	if (distance < attackDistance)
	{
		// 공격 상태로 전이하고싶다.
		SetState(EEnemyState::Attack);
		enemyAnim->bAttackPlay = false;
	}
}

void UEnemyFSM::TickAttack()
{

	currentTime += GetWorld()->GetDeltaSeconds();	// 시간이 흐르다가
	if (currentTime > attackWaitDelay)	// attackWaitDelay초가 지나면 
	{
		// 공격행위를 다시 할때 공격여부를 결정하는 시점.
		// 플레이어와의 거리를 구하고
		float distance = (target->GetActorLocation() - me->GetActorLocation()).Size();
		// 거리가 공격유효거리보다 멀어졌다면
		if (distance >= attackDistance) {
			// 이동상태로 전이하고싶다.
			SetState(EEnemyState::Move);
			enemyAnim->bAttackPlay = false;
		}
		else
		{
			// 공격행위를 다시 하겠다.
			enemyAnim->bAttackPlay = true;
		}
		currentTime = 0;// 현재시간을 초기화 하고싶다.
	}

}

void UEnemyFSM::TickDamage()
{
}

void UEnemyFSM::TickDie()
{
	if (false == bDieEndGoDown)
	{
		return;
	}

	// 2초동안 아래로 이동 후 
	currentTime += GetWorld()->GetDeltaSeconds();

	// 아래로 이동처리
	me->SetActorLocation(me->GetActorLocation() + FVector::DownVector * 100 * GetWorld()->GetDeltaSeconds());

	if (currentTime > 2) {
		me->Destroy();
	}
}

void UEnemyFSM::SetStateDamage()
{
	SetState(EEnemyState::Damage);

	ai->StopMovement();

	// 몽타주 플레이 
	// Section : Damage0, Damage1
	int index = FMath::RandRange(0, 1);
	FString sectionName = FString::Printf(TEXT("Damage%d"), index);
	me->PlayAnimMontage(enemyMontage, 1, FName(*sectionName));

}

void UEnemyFSM::SetStateDie()
{
	SetState(EEnemyState::Die);

	ai->StopMovement();

	currentTime = 0;
	// 충돌체를 끄고싶다.
	auto col = me->GetCapsuleComponent();
	col->SetCollisionProfileName(TEXT("NoCollision"));

	// 몽타주 플레이 
	// Section : Die
	me->PlayAnimMontage(enemyMontage, 1, TEXT("Die"));
}

void UEnemyFSM::OnMyAnimDamageEnd()
{
	SetState(EEnemyState::Move);
}

void UEnemyFSM::OnMyAnimDieEnd()
{
	bDieEndGoDown = true;
	currentTime = 0;
}

/// <summary>
/// 상태가 전이되는 순간에 호출해야한다.
/// </summary>
/// <param name="next">다음 상태</param>
void UEnemyFSM::SetState(EEnemyState next)
{
	// 내 상태를 next로 하고싶다.
	state = next;
	// 애니메이션의 상태로 next로 하고싶다.
	enemyAnim->enemyState = next;
}

bool UEnemyFSM::UpdateRandomDestination(const FVector& origin, float radius, FVector& dest)
{
	auto naviSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = naviSystem->GetRandomReachablePointInRadius(origin, radius, loc);

	dest = loc.Location;

	return result;
}
