// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class MYTPS_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEnemyFSM* enemyFSM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int hp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int maxHP = 2;

	void OnDamageProcess(int damageValue);

	UFUNCTION(BlueprintImplementableEvent)
	void OnMyPlayAnimDamage();

	UFUNCTION(BlueprintImplementableEvent)
	void OnMyPlayAnimDie();

};
