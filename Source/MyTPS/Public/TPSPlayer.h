// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

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

	// 스프링암, 카메라를 배치하고싶다.
	UPROPERTY(EditAnywhere)
	class USpringArmComponent* springArmComp;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* cameraComp;


	void OnAxisHorizontal(float value);
	void OnAxisVertical(float value);
	void OnAxisMouseX(float value);
	void OnAxisMouseY(float value);
	void OnActionJump();
	void OnActionFirePressed();
	void OnActionFireReleased();
	void OnMyMakeBullet();
	FTimerHandle autoFireTimerHandle;

	// Fire버튼을 누르면 총알공장에서 총알을 만들고
	// gunMeshComp의 FirePosition에 배치하고싶다.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletActor> bulletFactory;


	FVector direction;

	// 총 Mesh를 몸에 붙이고싶다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USkeletalMeshComponent* gunMeshComp;
};
