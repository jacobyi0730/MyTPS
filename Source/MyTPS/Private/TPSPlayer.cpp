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
#include "PlayerMoveComp.h"
#include "PlayerFireComp.h"

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
		gunMeshComp->SetRelativeLocationAndRotation(FVector(-8.1f, 6.8f, -2.5f), FRotator(6, 106, 3));
	
	}

	// sniperMeshComp를 만들고 에셋도 적용하고싶다.
	sniperMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("sniperMeshComp"));
	sniperMeshComp->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempSniper(TEXT("StaticMesh'/Game/Res/SniperGun/sniper1.sniper1'"));
	if (tempSniper.Succeeded())
	{
		sniperMeshComp->SetStaticMesh(tempSniper.Object);
		sniperMeshComp->SetRelativeLocationAndRotation(FVector(-27.7f, 0.12f, -0.5f), FRotator(6, 106.5f, 3));
		sniperMeshComp->SetRelativeScale3D(FVector(0.15f));
	}

	ConstructorHelpers::FObjectFinder<USoundBase> tempFireSound(TEXT("SoundWave'/Game/Res/SniperGun/Rifle.Rifle'"));

	if (tempFireSound.Succeeded())
	{
		fireSound = tempFireSound.Object;
	}

	moveComp = CreateDefaultSubobject<UPlayerMoveComp>(TEXT("moveComp"));

	fireComp = CreateDefaultSubobject<UPlayerFireComp>(TEXT("fireComp"));
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();


}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	moveComp->SetupInput(PlayerInputComponent);
	fireComp->SetupInput(PlayerInputComponent);
}

