// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyraCharacterAnimInstance.h"

#include "SkyraCharacter.h"
#include "SkyraCharacterMovementComponent.h"

USkyraCharacterAnimInstance::USkyraCharacterAnimInstance(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
}

void USkyraCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	const ASkyraCharacter* Character = Cast<ASkyraCharacter>(GetOwningActor());
	if (!Character)
	{
		return;
	}

	USkyraCharacterMovementComponent* CharMoveComp = CastChecked<USkyraCharacterMovementComponent>(Character->GetCharacterMovement());
	const FSkyraCharacterGroundInfo& GroundInfo = CharMoveComp->GetGroundInfo();
	GroundDistance = GroundInfo.GroundDistance;
}

