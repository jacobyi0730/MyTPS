// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
// 컴파일러 경고(수준 4) C4458 
//  : 맴버변수의 이름과 로컬변수의 이름이 같으면 경고
// https://learn.microsoft.com/ko-kr/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4458?view=msvc-170
// 해당 경고를 무시하려면
#pragma warning(disable: 4458) 

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle UMETA(Displayname= IDLE),
	Move UMETA(Displayname = MOVE),
	Attack UMETA(Displayname = ATTACK),
	Damage UMETA(Displayname = DAMAGE),
	Die UMETA(Displayname = DIE)
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTPS_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EEnemyState state;

	UPROPERTY()
	class AEnemy* me;

	UPROPERTY()
	class ATPSPlayer* target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackDistance = 160.f;

public:
	void TickIdle();
	void TickMove();
	void TickAttack();
	void TickDamage();
	void TickDie();

	float currentTime;

	void SetStateDamage();
	void SetStateDie();

	// 태어날 때 EnemyAnim를 가져와서 enemyAnim변수에 넣어놓고싶다.
	// 살아가면서 나의 state를 EnemyAnim의 enemyState에 값을 넣어주고싶다.
	UPROPERTY()
	class UEnemyAnim* enemyAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float attackWaitDelay = 3;

	UFUNCTION(BlueprintCallable)
	void OnMyAnimDamageEnd();
	
	UFUNCTION(BlueprintCallable)
	void OnMyAnimDieEnd();

	bool bDieEndGoDown = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAnimMontage* enemyMontage;

	void SetState(EEnemyState next);

	UPROPERTY(EditAnywhere)
	class AAIController* ai;

	FVector destination;

	bool UpdateRandomDestination(const FVector& orgin, float radius, FVector& dest);
};
