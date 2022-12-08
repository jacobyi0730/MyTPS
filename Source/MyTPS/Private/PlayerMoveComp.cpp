// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMoveComp.h"

// Sets default values for this component's properties
UPlayerMoveComp::UPlayerMoveComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerMoveComp::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerMoveComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerMoveComp::SetupInput(class UInputComponent* PlayerInputComponent)
{
	// 이동, 회전, 점프에 대한 함수를 Bind 하고싶다.
	PlayerInputComponent->BindAxis(TEXT("Move Forward / Backward"), this, &UPlayerMoveComp::OnAxisVertical);
	PlayerInputComponent->BindAxis(TEXT("Move Right / Left"), this, &UPlayerMoveComp::OnAxisHorizontal);
	PlayerInputComponent->BindAxis(TEXT("Turn Right / Left Mouse"), this, &UPlayerMoveComp::OnAxisMouseX);
	PlayerInputComponent->BindAxis(TEXT("Look Up / Down Mouse"), this, &UPlayerMoveComp::OnAxisMouseY);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &UPlayerMoveComp::OnActionJump);
}

