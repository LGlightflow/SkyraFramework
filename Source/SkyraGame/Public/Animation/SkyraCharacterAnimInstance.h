// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkyraAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "SkyraCharacterAnimInstance.generated.h"

/*
UENUM(BlueprintType)
enum class EAnimEnum_RootYawOffsetMode: uint8
{
	BlendOut   UMETA(DisplayName = "BlendOut"),
	Hold       UMETA(DisplayName = "Hold"),
	Accumulate UMETA(DisplayName = "Accumulate")
};

UENUM(BlueprintType)
enum class EAnimEnum_CardinalDirection: uint8
{
	Forward   UMETA(DisplayName = "Forward"),
	Backward     UMETA(DisplayName = "Backward"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right")
};
*/

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
	
	// 考虑到蓝图线程安全问题先不在C++中写
	/*
protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance")
	bool bIsFirstUpdate = true;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance")
	bool bEnableControlRig = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance")
	bool bUseFootPlacement  = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance")
	bool bEnableRootYawOffset = true;
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Rotation Data")
	FRotator WorldRotation = FRotator(0.0f, 0.0f, 0.0f);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Rotation Data")
	float  YawDeltaSinceLastUpdate = 0.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Rotation Data")
	float AdditiveLeanAngle = 0.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Rotation Data")
	float YawDeltaSpeed = 0.0f;
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Location Data")
	FVector WorldLocation = FVector(0.0f, 0.0f, 0.0f);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Location Data")
	float DisplacementSinceLastUpdate = 0.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Location Data")
	float DisplacementSpeed = 0.0f;
	
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Velocity Data")
	FVector WorldVelocity = FVector(0.0f, 0.0f, 0.0f);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Velocity Data")
	FVector LocalVelocity2D = FVector(0.0f, 0.0f, 0.0f);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Velocity Data")
	float LocalVelocityDirectionAngle = 0.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Velocity Data")
	float LocalVelocityDirectionAngleWithOffset= 0.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Velocity Data")
	EAnimEnum_CardinalDirection LocalVelocityDirection = EAnimEnum_CardinalDirection::Forward;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Velocity Data")
	EAnimEnum_CardinalDirection LocalVelocityDirectionNoOffset = EAnimEnum_CardinalDirection::Forward;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Velocity Data")
	bool bHasVelocity =	false;
	
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Acceleration Data")
	FVector LocalAcceleration2D = FVector(0.0f, 0.0f, 0.0f);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Acceleration Data")
	bool bHasAcceleration =	false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Acceleration Data")
	FVector PivotDirection2D = FVector(0.0f, 0.0f, 0.0f);
	
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Character State Data")
	bool bIsOnGround = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Character State Data")
	bool bIsCrouching = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Character State Data")
	bool bCrouchStateChange = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Character State Data")
	bool bADSStateChanged = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Character State Data")
	bool bWasADSLastUpdate =false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Character State Data")
	float TimeSinceFiredWeapon = 9999.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Character State Data")
	bool bIsJumping = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Character State Data")
	bool bIsFalling = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Character State Data")
	float TimeToJumpApex = 0.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Character State Data")
	bool bIsRunningIntoWall = false;
	
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Gameplay Tag Bindings")
	bool bGameplayTag_IsADS = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Gameplay Tag Bindings")
	bool bGameplayTag_IsFiring = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Gameplay Tag Bindings")
	bool GameplayTag_IsReloading = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Gameplay Tag Bindings")
	bool GameplayTag_IsDashing = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Gameplay Tag Bindings")
	bool GameplayTag_IsMelee = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Locomotion SM Data")
	EAnimEnum_CardinalDirection LocomotionDirection = EAnimEnum_CardinalDirection::Forward;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Locomotion SM Data")
	EAnimEnum_CardinalDirection PivotInitialDirection = EAnimEnum_CardinalDirection::Forward;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Locomotion SM Data")
	float LastPivotTime = 0.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Locomotion SM Data")
	EAnimEnum_CardinalDirection CardinalDirectionFromAcceleration = EAnimEnum_CardinalDirection::Forward;
	
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Blend Weight Data")
	float UpperbodyDynamicAdditiveWeight = 0.0f;
	
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Aiming Data")
	float AimPitch = 0.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Aiming Data")
	float AimYaw = 0.0f;
	
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Settings")
	float CardinalDirectionDeadZone = 0.0f;
	
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Linked Layer Data")
	bool bLinkedLayerChanged = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Linked Layer Data")
	UAnimInstance* LastLinkedLayer = nullptr;
	
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Turn In Place")
	float RootYawOffset = 0.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Turn In Place")
	FFloatSpringState RootYawOffsetSpringState;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Turn In Place")
	float TurnYawCurveValue = 0.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Skyra|AnimInstance|Turn In Place")
	EAnimEnum_RootYawOffsetMode RootYawOffsetMode = EAnimEnum_RootYawOffsetMode::BlendOut;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Skyra|AnimInstance|Turn In Place",meta=(UIMin="-180", UIMax="180", ClampMin="-180", ClampMax="180"))
	FVector2D RootYawOffsetAngleClamp = FVector2D(-120.0f, 100.0f);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Skyra|AnimInstance|Turn In Place",meta=(UIMin="-180", UIMax="180", ClampMin="-180", ClampMax="180"))
	FVector2D RootYawOffsetAngleClampCrouched = FVector2D(-90.0f, 80.0f);
	
protected:
	UFUNCTION(BlueprintCallable, Category="Skyra|AnimInstance|State Node Functions")
	void UpdateIdleState(const FAnimUpdateContext& Context,const FAnimNodeReference& Node);
	
	UFUNCTION(BlueprintCallable,Category="Skyra|AnimInstance")
	void UpdateLocationData(float DeltaTime);
	
	*/
	
};
