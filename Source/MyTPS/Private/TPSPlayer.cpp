// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "BulletActor.h"
#include <Blueprint/UserWidget.h>
#include <Kismet/GameplayStatics.h>
#include <Particles/ParticleSystem.h>
#include "Enemy.h"
#include <Camera/PlayerCameraManager.h>
#include <GameFramework/PlayerController.h>

// Sets default values
ATPSPlayer::ATPSPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// USkeletalMesh(Asset)를 가져와서 내 Mesh에 적용하고싶다.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempBody(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn.SKM_Quinn'"));

	if (tempBody.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempBody.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	}

	// 스프링암 컴포넌트와 카메라 컴포넌트를 생성하고싶다.
	springArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("springArmComp"));
	cameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("cameraComp"));
	// 스프링암은 RootComponent에 Attatch하고싶다.
	springArmComp->SetupAttachment(RootComponent);
	// 카메라는 스프링암에 Attatch하고싶다.
	cameraComp->SetupAttachment(springArmComp);
	// 스프링암의 위치와 targetArmLength를 설정하고싶다.
	springArmComp->SetRelativeLocation(FVector(0, 40, 70));
	springArmComp->TargetArmLength = 100;


	this->bUseControllerRotationYaw = true;
	springArmComp->bUsePawnControlRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	gunMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("gunMeshComp"));
	// 생성된 총을 몸Mesh에 붙이고싶다.
	gunMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGunMesh(TEXT("SkeletalMesh'/Game/Res/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (tempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(tempGunMesh.Object);
		// gunMeshComp위치를 설정하고싶다.
		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}

	// sniperMeshComp를 만들고 에셋도 적용하고싶다.
	sniperMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sniperMeshComp"));
	sniperMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper(TEXT("StaticMesh'/Game/Res/SniperGun/sniper1.sniper1'"));
	if (tempSniper.Succeeded())
	{
		sniperMeshComp->SetStaticMesh(tempSniper.Object);
		sniperMeshComp->SetRelativeLocation(FVector(-22, 55, 120));
		sniperMeshComp->SetRelativeScale3D(FVector(0.15f));
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempFireSound(TEXT("SoundWave'/Game/Res/SniperGun/Rifle.Rifle'"));

	if (tempFireSound.Succeeded())
	{
		fireSound = tempFireSound.Object;
	}
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();


	sniperUI = CreateWidget(GetWorld(), sniperUIFactory);
	crosshairUI = CreateWidget(GetWorld(), crosshairUIFactory);

	OnActionChooseGun();
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	direction = FTransform(GetControlRotation()).TransformVector(direction);
	AddMovementInput(direction);
	direction = FVector::ZeroVector;
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 이동, 회전, 점프에 대한 함수를 Bind 하고싶다.
	PlayerInputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &ATPSPlayer::OnAxisVertical);
	PlayerInputComponent->BindAxis(TEXT("Move Right / Left"), this, &ATPSPlayer::OnAxisHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Turn Right / Left Mouse"), this, &ATPSPlayer::OnAxisMouseX);
	PlayerInputComponent->BindAxis(TEXT("Look Up / Down Mouse"), this, &ATPSPlayer::OnAxisMouseY);
	
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ATPSPlayer::OnActionJump);

	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &ATPSPlayer::OnActionFirePressed);

	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Released, this, &ATPSPlayer::OnActionFireReleased);

	PlayerInputComponent->BindAction(TEXT("ChooseGun"), EInputEvent::IE_Pressed, this, &ATPSPlayer::OnActionChooseGun);

	PlayerInputComponent->BindAction(TEXT("ChooseSniperGun"), EInputEvent::IE_Pressed, this, &ATPSPlayer::OnActionChooseSniperGun);


	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Pressed, this, &ATPSPlayer::OnActionZoomIn);

	PlayerInputComponent->BindAction(TEXT("Zoom"), IE_Released, this, &ATPSPlayer::OnActionZoomOut);
}

void ATPSPlayer::OnAxisHorizontal(float value)
{
	direction.Y = value;	// 좌우
}

void ATPSPlayer::OnAxisVertical(float value)
{
	direction.X = value;	// 앞뒤
}

void ATPSPlayer::OnAxisMouseX(float value)
{
	AddControllerYawInput(value);
}

void ATPSPlayer::OnAxisMouseY(float value)
{
	AddControllerPitchInput(value);
}

void ATPSPlayer::OnActionJump()
{
	ACharacter::Jump();
}

void ATPSPlayer::OnActionFirePressed()
{
	if (true == bSniperGun) {
		PlayAttackAnim();

		LineShot();
	}
	else { // 유탄총쏘기
		GetWorldTimerManager().SetTimer(autoFireTimerHandle, this, &ATPSPlayer::OnMyMakeBullet, 0.5f, true, 0.5f);

		OnMyMakeBullet();
	}
}

void ATPSPlayer::OnActionFireReleased()
{
	if (false == bSniperGun) {
		GetWorldTimerManager().ClearTimer(autoFireTimerHandle);
	}
}

void ATPSPlayer::OnMyMakeBullet()
{
	PlayAttackAnim();

	// FirePosition을 구하고
	FTransform t = gunMeshComp->GetSocketTransform(TEXT("FirePosition"));
	// 총알공장으로 총알을 Spawn하고싶다.
	GetWorld()->SpawnActor<ABulletActor>(bulletFactory, t);
}

void ATPSPlayer::OnActionChooseGun() {
	// 유탄총만 보이게 하고싶다.
	bSniperGun = false;
	gunMeshComp->SetVisibility(true);
	sniperMeshComp->SetVisibility(false);
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

void ATPSPlayer::OnActionChooseSniperGun() {
	// 스나이퍼 총만 보이게 하고싶다.
	bSniperGun = true;
	gunMeshComp->SetVisibility(false);
	sniperMeshComp->SetVisibility(true);
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

void ATPSPlayer::OnActionZoomIn() {
	
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
	cameraComp->SetFieldOfView(30);
}
void ATPSPlayer::OnActionZoomOut() {
	
	// FOV 를 90으로 하고싶다.
	cameraComp->SetFieldOfView(90);

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

void ATPSPlayer::LineShot()
{
	FVector start = cameraComp->GetComponentLocation();
	FVector end = start + cameraComp->GetForwardVector() * 300000;
	FHitResult hitInfo;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

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

void ATPSPlayer::PlayAttackAnim()
{
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), fireSound, GetActorLocation());

	PlayAnimMontage(attackAnimMontage);

	if (nullptr == cameraShake || cameraShake->IsFinished())
	{
		cameraShake = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(cameraShakeFactory);
	}
}

