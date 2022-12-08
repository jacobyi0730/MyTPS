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

	// 일정시간마다 적을 생성하고싶다.
	GetWorldTimerManager().SetTimer(timerHandleMakeEnemy, this, &ASpawnManager::MakeEnemy, makeTime, true);
	
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnManager::MakeEnemy()
{
	// 적공장에서 적을 만들어서 랜덤위치목록중에서 랜덤한 위치를 하나 골라서 그 곳에 배치하고싶다.
	
	// 랜덤위치목록중에서 랜덤한 위치를 하나 골라서
	int index = FMath::RandRange(0, spawnList.Num() - 1);
	FTransform transform = spawnList[index]->GetActorTransform();
	// 적공장에서 적을 만들어서 고른 위치에 배치하고싶다.
	GetWorld()->SpawnActor<AEnemy>(enemyFactory, transform);
}

