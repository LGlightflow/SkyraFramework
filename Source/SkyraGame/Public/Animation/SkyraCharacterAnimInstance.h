// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkyraAnimInstance.h"
#include "SkyraCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS(Config = Game)
class SKYRAGAME_API USkyraCharacterAnimInstance : public USkyraAnimInstance
{
	GENERATED_BODY()
	
public:
	USkyraCharacterAnimInstance(const FObjectInitializer& ObjectInitializer);
	
protected:
	
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
