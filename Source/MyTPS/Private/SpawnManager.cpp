// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include "Enemy.h"
#include <Kismet/GameplayStatics.h>
#include "SpawnActor.h"

// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FClassFinder<AEnemy> tempEnemy(TEXT("Blueprint'/Game/Blueprints/BP_Enemy.BP_Enemy_C'"));
	if (tempEnemy.Succeeded())
	{
		enemyFactory = tempEnemy.Class;
	}
}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnActor::StaticClass(), spawnList);

	// �����ð����� ���� �����ϰ�ʹ�.
	GetWorldTimerManager().SetTimer(timerHandleMakeEnemy, this, &ASpawnManager::MakeEnemy, makeTime, true);
	
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnManager::MakeEnemy()
{
	// �����忡�� ���� ���� ������ġ����߿��� ������ ��ġ�� �ϳ� ��� �� ���� ��ġ�ϰ�ʹ�.
	
	// ������ġ����߿��� ������ ��ġ�� �ϳ� ���
	int index = FMath::RandRange(0, spawnList.Num() - 1);
	FTransform transform = spawnList[index]->GetActorTransform();
	// �����忡�� ���� ���� �� ��ġ�� ��ġ�ϰ�ʹ�.
	GetWorld()->SpawnActor<AEnemy>(enemyFactory, transform);
}

