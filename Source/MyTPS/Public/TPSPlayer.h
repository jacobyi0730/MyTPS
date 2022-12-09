// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UInputComponent*);

UCLASS()
class MYTPS_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FInputBindingDelegate onInputBindingDelegate;

	// 기존 이동, 총쏘기 기능을 컴포넌트로 옮기고 싶다.
	UPROPERTY(EditAnywhere)
	class UPlayerBaseComp* moveComp;

	UPROPERTY(EditAnywhere)
	class UPlayerBaseComp* fireComp;

	// 스프링암, 카메라를 배치하고싶다.
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UCameraComponent* cameraComp;

	// 총 Mesh를 몸에 붙이고싶다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* gunMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStaticMeshComponent* sniperMeshComp;

	UPROPERTY(EditAnywhere)
	class USoundBase* fireSound;

	
	UPROPERTY(EditAnywhere)
	int hp;

	UPROPERTY(EditAnywhere)
	int maxHP = 3;

	// 적이 나(플레이어)를 공격(Hit)하면 체력을 감소시키고싶다.
	void OnMyHitFromEnemy(int damage);

};
