// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

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
	gunMeshComp->SetupAttachment(GetMesh());

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempGunMesh(TEXT("SkeletalMesh'/Game/Res/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));

	if (tempGunMesh.Succeeded())
	{
		gunMeshComp->SetSkeletalMesh(tempGunMesh.Object);
		// gunMeshComp위치를 설정하고싶다.
		gunMeshComp->SetRelativeLocation(FVector(-14, 52, 120));
	}
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

