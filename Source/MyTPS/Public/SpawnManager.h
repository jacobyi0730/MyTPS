// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

UCLASS()
class MYTPS_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// �����ð����� �����忡�� ���� ���� ������ġ����߿��� ������ ��ġ�� �ϳ� ��� �� ���� ��ġ�ϰ�ʹ�.

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AEnemy> enemyFactory;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> spawnList;

	FTimerHandle timerHandleMakeEnemy;
	void MakeEnemy();
	float makeTime = 3;

};
