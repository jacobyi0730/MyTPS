// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerFireComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTPS_API UPlayerFireComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerFireComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupInput(class UInputComponent* PlayerInputComponent);

	UPROPERTY()
	class ATPSPlayer* me;


		
		// 총쏘기 관련
	void OnActionFirePressed();
	void OnActionFireReleased();
	void OnMyMakeBullet();
	FTimerHandle autoFireTimerHandle;

	// Fire버튼을 누르면 총알공장에서 총알을 만들고
	// gunMeshComp의 FirePosition에 배치하고싶다.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABulletActor> bulletFactory;

	bool bSniperGun = false;
	void OnActionChooseGun();
	void OnActionChooseSniperGun();


	// 시작할 때 sniperUI를 생성해야함.
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> sniperUIFactory;

	UPROPERTY()
	class UUserWidget* sniperUI;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> crosshairUIFactory;

	UPROPERTY()
	class UUserWidget* crosshairUI;

	// Zoom 키처리 함수 제작
	void OnActionZoomIn();
	void OnActionZoomOut();

	// 마우스 오른쪽 버튼 누르면 총 별로 따로 처리하고싶다.
	// 라인쏘기를 하면 부딪힌 곳에 총알 자국을 표시하고싶다.
	UPROPERTY(EditAnywhere)
	class UParticleSystem* bulletImpactFactory;

	void LineShot();

	UPROPERTY(EditAnywhere)
	class UAnimMontage* attackAnimMontage;

	void PlayAttackAnim();


	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCameraShakeBase> cameraShakeFactory;
	
	UPROPERTY()
	UCameraShakeBase* cameraShake;
};
