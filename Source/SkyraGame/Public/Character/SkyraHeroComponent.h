// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkyraPawnControlComponent.h"
#include "SkyraHeroComponent.generated.h"


UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class SKYRAGAME_API USkyraHeroComponent : public USkyraPawnControlComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USkyraHeroComponent(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	
	
	virtual  void InitializePlayerInput(UInputComponent* PlayerInputComponent) override;
protected:
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_LookMouse(const FInputActionValue& InputActionValue);
	void Input_LookStick(const FInputActionValue& InputActionValue);
	void Input_Crouch(const FInputActionValue& InputActionValue);
	void Input_AutoRun(const FInputActionValue& InputActionValue);
};
