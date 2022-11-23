// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	UPROPERTY(EditAnywhere)
	EEnemyState state;

	UPROPERTY()
	class AEnemy* me;
};
