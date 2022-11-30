// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"

// Sets default values
AEnemy::AEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 외형을 로드해서 적용하고싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempBody(TEXT("SkeletalMesh'/Game/Res/Enemy/Model/vampire_a_lusth.vampire_a_lusth'"));
	if (tempBody.Succeeded()) {
		GetMesh()->SetSkeletalMesh(tempBody.Object);
		// 위치 : 0, 0, -87
		// 회전 : 0, -90, 0
		// 크기 : 0.84f
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -87), FRotator(0, -90, 0));
		GetMesh()->SetRelativeScale3D(FVector(0.84f));
	}

	// enemyFSM 컴포넌트를 생성하고싶다.
	enemyFSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("enemyFSM"));
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	// 태어날 때 체력을 최대체력으로 하고싶다.
	hp = maxHP;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::OnDamageProcess(int damageValue)
{
	// 체력을 줄이고싶다.
	hp -= damageValue;
	// 만약 체력이 0보다 크다면
	if (hp > 0) {
		// FSM의 상태를 Damage로 하고싶다.
		enemyFSM->SetStateDamage();
		OnMyPlayAnimDamage();
	}
	// 그렇지 않다면(체력이 0이하라면)
	else {
		// FSM의 상태를 Die로 하고싶다.
		enemyFSM->SetStateDie();
		OnMyPlayAnimDie();
	}
}

