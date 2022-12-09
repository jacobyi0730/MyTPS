// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerBaseComp.h"
#include "PlayerMoveComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYTPS_API UPlayerMoveComp : public UPlayerBaseComp
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerMoveComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetupInput(class UInputComponent* PlayerInputComponent) override;

	// 이동 관련
	void OnAxisHorizontal(float value);
	void OnAxisVertical(float value);
	void OnAxisMouseX(float value);
	void OnAxisMouseY(float value);
	void OnActionJump();
	FVector direction;
};
