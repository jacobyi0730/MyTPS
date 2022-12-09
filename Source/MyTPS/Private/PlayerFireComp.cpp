// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerFireComp.h"
#include "TPSPlayer.h"
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include "Enemy.h"
#include <Camera/CameraComponent.h>
#include <Camera/PlayerCameraManager.h>
#include "BulletActor.h"

// Sets default values for this component's properties
UPlayerFireComp::UPlayerFireComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerFireComp::BeginPlay()
{
	Super::BeginPlay();

	
	
	sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);

	OnActionChooseSniperGun();

}


// Called every frame
void UPlayerFireComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerFireComp::SetupInput(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &UPlayerFireComp::OnActionFirePressed);

	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &UPlayerFireComp::OnActionFireReleased);

	PlayerInputComponent->BindAction(TEXT("ChooseGun"), EInputEvent::IE_Pressed, this, &UPlayerFireComp::OnActionChooseGun);

	PlayerInputComponent->BindAction(TEXT("ChooseSniperGun"), EInputEvent::IE_Pressed, this, &UPlayerFireComp::OnActionChooseSniperGun);


	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &UPlayerFireComp::OnActionZoomIn);

	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &UPlayerFireComp::OnActionZoomOut);
}



void UPlayerFireComp::OnActionFirePressed()
{
	if (true == bSniperGun) {
		PlayAttackAnim();

		LineShot();
	}
	else { // 유탄총쏘기
		me->GetWorldTimerManager().SetTimer(autoFireTimerHandle, this, &UPlayerFireComp::OnMyMakeBullet, 0.5f, true, 0.5f);

		OnMyMakeBullet();
	}
}

void UPlayerFireComp::OnActionFireReleased()
{
	if (false == bSniperGun) {
		me->GetWorldTimerManager().ClearTimer(autoFireTimerHandle);
	}
}

void UPlayerFireComp::OnMyMakeBullet()
{
	PlayAttackAnim();

	// FirePosition을 구하고
	FTransform t = me->gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	// 총알공장으로 총알을 Spawn하고싶다.
	GetWorld()->SpawnActor<ABulletActor>(bulletFactory, t);
}

void UPlayerFireComp::OnActionChooseGun() {
	// 유탄총만 보이게 하고싶다.
	bSniperGun = false;
	me->gunMeshComp->SetVisibility(true);
	me->sniperMeshComp->SetVisibility(false);
	// crosshairUI와 sniperUI를 보이지않게 하고싶다.
	if (crosshairUI && crosshairUI->IsInViewport())
	{
		crosshairUI->RemoveFromParent();
	}
	if (sniperUI && sniperUI->IsInViewport())
	{
		sniperUI->RemoveFromParent();
	}

}

void UPlayerFireComp::OnActionChooseSniperGun() {
	// 스나이퍼 총만 보이게 하고싶다.
	bSniperGun = true;
	me->gunMeshComp->SetVisibility(false);
	me->sniperMeshComp->SetVisibility(true);
	// crosshairUI는 보이고 sniperUI는 안보이게 하고싶다.
	if (crosshairUI && false == crosshairUI->IsInViewport())
	{
		crosshairUI->AddToViewport();
	}
	if (sniperUI && sniperUI->IsInViewport())
	{
		sniperUI->RemoveFromViewport();
	}
}

void UPlayerFireComp::OnActionZoomIn() {

	// 만약 1번총 상태라면 함수를 바로 반환하고싶다.
	if (false == bSniperGun)
	{
		return;
	}
	// sniperUI를 보이게하고,crosshairUI는 안보이게 하고싶다.
	if (sniperUI && false == sniperUI->IsInViewport())
	{
		sniperUI->AddToViewport();
	}
	if (crosshairUI && true == crosshairUI->IsInViewport())
	{
		crosshairUI->RemoveFromParent();
	}
	// FOV 를 30으로 하고싶다.
	me->cameraComp->SetFieldOfView(30);
}
void UPlayerFireComp::OnActionZoomOut() {

	// FOV 를 90으로 하고싶다.
	me->cameraComp->SetFieldOfView(90);

	// 만약 1번총 상태라면 함수를 바로 반환하고싶다.
	if (false == bSniperGun)
	{
		return;
	}

	// sniperUI를 보이지 않게 하고 crosshairUI는 보이게 하고싶다.
	if (sniperUI && true == sniperUI->IsInViewport())
	{
		sniperUI->RemoveFromParent();
	}
	if (crosshairUI && false == crosshairUI->IsInViewport())
	{
		crosshairUI->AddToViewport();
	}
}

void UPlayerFireComp::LineShot()
{
	FVector start = me->cameraComp->GetComponentLocation();
	FVector end = start + me->cameraComp->GetForwardVector() * 300000;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(me);

	if (GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECollisionChannel::ECC_Visibility, params))
	{
		//UParticleSystem
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bulletImpactFactory, hitInfo.ImpactPoint);

		// 만약 부딪힌 물체의 물리가 켜져있다면
		auto hitComp = hitInfo.GetComponent();
		if (hitComp && hitComp->IsSimulatingPhysics())
		{
			// 그 물체에게 힘을 가하고싶다.
			FVector dir = (hitInfo.ImpactPoint - start);
			dir.Normalize();
			FVector force = dir * hitComp->GetMass() * 500000;
			hitComp->AddForce(force);
		}
		// 만약 부딪힌 액터가 Enemy라면
		//auto enemy = Cast<AEnemy>(hitInfo.GetActor());
		//if (nullptr != enemy)
		//{
		//	enemy->OnDamageProcess(1);
		//}
		auto hitActor = hitInfo.GetActor();
		if (hitActor)
		{
			if (hitActor->IsA(AEnemy::StaticClass()))
			{
				// Enemy의 OnDamageProcess 함수를 호출하고싶다.
				auto enemy = Cast<AEnemy>(hitActor);
				enemy->OnDamageProcess(1);
			}
		}

	}
}

void UPlayerFireComp::PlayAttackAnim()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), me->fireSound, me->GetActorLocation());

	me->PlayAnimMontage(attackAnimMontage);

	if (nullptr == cameraShake || cameraShake->IsFinished())
	{
		cameraShake = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(cameraShakeFactory);
	}
}

