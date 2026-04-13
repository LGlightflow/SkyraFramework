// Fill out your copyright notice in the Description page of Project Settings.


#include "SkyraCharacterAnimInstance.h"
#include "AnimationStateMachineLibrary.h"
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


/*
void USkyraCharacterAnimInstance::UpdateLocationData(float DeltaTime)
{
	// 1️.获取拥有者 Actor
	AActor* Owner = GetOwningActor();
	if (!Owner)
	{
		return;
	}

	// 2️.获取当前位置
	const FVector CurrentLocation = Owner->GetActorLocation();

	// 3️.计算位移（当前帧 - 上一帧）
	const FVector Delta = CurrentLocation - WorldLocation;

	// 4️.只计算水平位移（XY），忽略Z（Lyra设计重点）
	DisplacementSinceLastUpdate = FVector(Delta.X, Delta.Y, 0.f).Size();

	// 5️.更新缓存位置（供下一帧使用）
	WorldLocation = CurrentLocation;

	// 6️.计算速度（安全除法，避免 DeltaTime = 0）
	if (DeltaTime > KINDA_SMALL_NUMBER)
	{
		DisplacementSpeed = DisplacementSinceLastUpdate / DeltaTime;
	}
	else
	{
		DisplacementSpeed = 0.f;
	}

	// 7️.首帧特殊处理（Lyra核心逻辑）
	if (bIsFirstUpdate)
	{
		// 第一次没有“上一帧”，所以清零
		DisplacementSinceLastUpdate = 0.f;
		DisplacementSpeed = 0.f;
		
	}
}
*/

