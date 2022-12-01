// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class MYTPS_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyState enemyState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttackPlay; // 공격동작을 할 때 공격행위를 할것인가?

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackDistance = 160;
	
};
