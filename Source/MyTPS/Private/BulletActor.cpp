// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletActor.h"
#include <Components/SphereComponent.h>
#include <Components/StaticMeshComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>

// Sets default values
ABulletActor::ABulletActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("sphereCollision"));

	sphereCollision->SetSphereRadius(10);
	sphereCollision->SetCollisionProfileName(TEXT("BlockAll"));

	SetRootComponent(sphereCollision);
	// meshComp, movementComp로 생성하고싶다.

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComp"));

	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));

	if (tempMesh.Succeeded())
	{
		meshComp->SetStaticMesh(tempMesh.Object);
		meshComp->SetRelativeScale3D(FVector(0.2f));
		meshComp->SetCollisionProfileName(TEXT("NoCollision"));
	}

	meshComp->SetupAttachment(RootComponent);
	
	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("movementComp"));
	movementComp->SetUpdatedComponent(sphereCollision);
	movementComp->InitialSpeed = 5000;
	movementComp->MaxSpeed = 5000;
	movementComp->bShouldBounce = true;
	movementComp->Bounciness = 0.3f;


}

// Called when the game starts or when spawned
void ABulletActor::BeginPlay()
{
	Super::BeginPlay();
	
	// 타이머를 이용해서 2초후에 Die함수를 호출하고싶다.
	FTimerHandle handle;
	GetWorldTimerManager().SetTimer(handle, 
		FTimerDelegate::CreateLambda(
			[this]()->void
			{
				this->Destroy();
			}
		), 2, false);



	// 델리게이트 : 대리자
	// 변수인데 함수의 주소를 담을 수 있다.
	// 무명함수 + Lambda식
	// 반환자료형 (매개변수);
	// 람다
	// 캡처
	
	auto lambdaFunction = [this]
	()->int
	{
		return 10;
	};

	int a = lambdaFunction();
}

// Called every frame
void ABulletActor::Die()
{
	this->Destroy();
}

void ABulletActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


