// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Curves/CurveFloat.h"

#include "SkyraWeaponInstance.h"
#include "AbilitySystem/SkyraAbilitySourceInterface.h"

#include "SkyraRangedWeaponInstance.generated.h"

class UPhysicalMaterial;

/**
 * USkyraRangedWeaponInstance
 *
 * A piece of equipment representing a ranged weapon spawned and applied to a pawn
 * 表示一个远程武器的装备实例，该武器会生成并应用到一个Pawn上
 */
UCLASS()
class USkyraRangedWeaponInstance : public USkyraWeaponInstance, public ISkyraAbilitySourceInterface
{
	GENERATED_BODY()

public:
	USkyraRangedWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	// 加载后调用
	virtual void PostLoad() override;

#if WITH_EDITOR
	// 编辑器中属性更改后调用
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;

	void UpdateDebugVisualization();
#endif
	
	// 获取每个弹夹的子弹数量
	int32 GetBulletsPerCartridge() const
	{
		return BulletsPerCartridge;
	}
	
	/** Returns the current spread angle (in degrees, diametrical) */
	/** 获取当前的扩散角度（以度为单位，直径角） */
	float GetCalculatedSpreadAngle() const
	{
		return CurrentSpreadAngle;
	}
	
	/** 获取当前的扩散角度乘数 */
	float GetCalculatedSpreadAngleMultiplier() const
	{
		return bHasFirstShotAccuracy ? 0.0f : CurrentSpreadAngleMultiplier;
	}
	
	// 检查是否具有首发精准度
	bool HasFirstShotAccuracy() const
	{
		return bHasFirstShotAccuracy;
	}
	
	// 获取扩散指数
	float GetSpreadExponent() const
	{
		return SpreadExponent;
	}
	
	// 获取最大伤害范围
	float GetMaxDamageRange() const
	{
		return MaxDamageRange;
	}
	
	// 获取子弹轨迹扫描半径
	float GetBulletTraceSweepRadius() const
	{
		return BulletTraceSweepRadius;
	}

protected:
#if WITH_EDITORONLY_DATA
	UPROPERTY(VisibleAnywhere, Category = "Spread|Fire Params")
	float Debug_MinHeat = 0.0f;

	UPROPERTY(VisibleAnywhere, Category = "Spread|Fire Params")
	float Debug_MaxHeat = 0.0f;

	UPROPERTY(VisibleAnywhere, Category="Spread|Fire Params", meta=(ForceUnits=deg))
	float Debug_MinSpreadAngle = 0.0f;

	UPROPERTY(VisibleAnywhere, Category="Spread|Fire Params", meta=(ForceUnits=deg))
	float Debug_MaxSpreadAngle = 0.0f;

	UPROPERTY(VisibleAnywhere, Category="Spread Debugging")
	float Debug_CurrentHeat = 0.0f;

	UPROPERTY(VisibleAnywhere, Category="Spread Debugging", meta = (ForceUnits=deg))
	float Debug_CurrentSpreadAngle = 0.0f;

	// The current *combined* spread angle multiplier
	UPROPERTY(VisibleAnywhere, Category = "Spread Debugging", meta=(ForceUnits=x))
	float Debug_CurrentSpreadAngleMultiplier = 1.0f;

#endif

	// Spread exponent, affects how tightly shots will cluster around the center line
	// when the weapon has spread (non-perfect accuracy). Higher values will cause shots
	// to be closer to the center (default is 1.0 which means uniformly within the spread range)
	// 扩散指数，影响武器扩散时（非完美精准度）子弹围绕中心线的聚集程度
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ClampMin=0.1), Category="Spread|Fire Params")
	float SpreadExponent = 1.0f;

	// A curve that maps the heat to the spread angle
	// The X range of this curve typically sets the min/max heat range of the weapon
	// The Y range of this curve is used to define the min and maximum spread angle
	// 热量到扩散角度的映射曲线
	UPROPERTY(EditAnywhere, Category = "Spread|Fire Params")
	FRuntimeFloatCurve HeatToSpreadCurve;

	// A curve that maps the current heat to the amount a single shot will further 'heat up'
	// This is typically a flat curve with a single data point indicating how much heat a shot adds,
	// but can be other shapes to do things like punish overheating by adding progressively more heat.
	// 热量到每次射击增加热量的映射曲线
	UPROPERTY(EditAnywhere, Category="Spread|Fire Params")
	FRuntimeFloatCurve HeatToHeatPerShotCurve;
	
	// A curve that maps the current heat to the heat cooldown rate per second
	// This is typically a flat curve with a single data point indicating how fast the heat
	// wears off, but can be other shapes to do things like punish overheating by slowing down
	// recovery at high heat.
	// 热量到每秒冷却速率的映射曲线
	UPROPERTY(EditAnywhere, Category="Spread|Fire Params")
	FRuntimeFloatCurve HeatToCoolDownPerSecondCurve;

	// Time since firing before spread cooldown recovery begins (in seconds)
	// 射击后扩散冷却恢复开始的延迟时间（以秒为单位）
	UPROPERTY(EditAnywhere, Category="Spread|Fire Params", meta=(ForceUnits=s))
	float SpreadRecoveryCooldownDelay = 0.0f;

	// Should the weapon have perfect accuracy when both player and weapon spread are at their minimum value
	// 是否允许首发精准度
	UPROPERTY(EditAnywhere, Category="Spread|Fire Params")
	bool bAllowFirstShotAccuracy = false;
	
	// Multiplier when in an aiming camera mode
	// 瞄准模式下的扩散角度乘数
	UPROPERTY(EditAnywhere, Category="Spread|Player Params", meta=(ForceUnits=x))
	float SpreadAngleMultiplier_Aiming = 1.0f;

	// Multiplier when standing still or moving very slowly
	// (starts to fade out at StandingStillSpeedThreshold, and is gone completely by StandingStillSpeedThreshold + StandingStillToMovingSpeedRange)
	// 静止或移动缓慢时的扩散角度乘数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params", meta=(ForceUnits=x))
	float SpreadAngleMultiplier_StandingStill = 1.0f;

	// Rate at which we transition to/from the standing still accuracy (higher values are faster, though zero is instant; @see FInterpTo)
	// 静止状态的过渡速率
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params")
	float TransitionRate_StandingStill = 5.0f;

	// Speeds at or below this are considered standing still
	// 静止速度阈值
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params", meta=(ForceUnits="cm/s"))
	float StandingStillSpeedThreshold = 80.0f;

	// Speeds no more than this above StandingStillSpeedThreshold are used to feather down the standing still bonus until it's back to 1.0
	// 静止到移动速度范围
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params", meta=(ForceUnits="cm/s"))
	float StandingStillToMovingSpeedRange = 20.0f;


	// Multiplier when crouching, smoothly blended to based on TransitionRate_Crouching
	// 蹲下时的扩散角度乘数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params", meta=(ForceUnits=x))
	float SpreadAngleMultiplier_Crouching = 1.0f;

	// Rate at which we transition to/from the crouching accuracy (higher values are faster, though zero is instant; @see FInterpTo)
	// 蹲下状态的过渡速率
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params")
	float TransitionRate_Crouching = 5.0f;


	// Spread multiplier while jumping/falling, smoothly blended to based on TransitionRate_JumpingOrFalling
	// 跳跃/下落时的扩散角度乘数
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params", meta=(ForceUnits=x))
	float SpreadAngleMultiplier_JumpingOrFalling = 1.0f;

	// Rate at which we transition to/from the jumping/falling accuracy (higher values are faster, though zero is instant; @see FInterpTo)
	// 跳跃/下落状态的过渡速率
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Spread|Player Params")
	float TransitionRate_JumpingOrFalling = 5.0f;

	// Number of bullets to fire in a single cartridge (typically 1, but may be more for shotguns)
	// 每个弹夹的子弹数量（通常为1，但霰弹枪可能更多）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Config")
	int32 BulletsPerCartridge = 1;

	// The maximum distance at which this weapon can deal damage
	// 武器可以造成伤害的最大距离
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Config", meta=(ForceUnits=cm))
	float MaxDamageRange = 25000.0f;

	// The radius for bullet traces sweep spheres (0.0 will result in a line trace)
	// 子弹轨迹扫描球体的半径（0.0表示线性扫描）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weapon Config", meta=(ForceUnits=cm))
	float BulletTraceSweepRadius = 0.0f;

	// A curve that maps the distance (in cm) to a multiplier on the base damage from the associated gameplay effect
	// If there is no data in this curve, then the weapon is assumed to have no falloff with distance
	// 距离到基础伤害乘数的映射曲线
	UPROPERTY(EditAnywhere, Category = "Weapon Config")
	FRuntimeFloatCurve DistanceDamageFalloff;

	// List of special tags that affect how damage is dealt
	// These tags will be compared to tags in the physical material of the thing being hit
	// If more than one tag is present, the multipliers will be combined multiplicatively
	// 特殊标签列表，影响伤害的处理方式
	UPROPERTY(EditAnywhere, Category = "Weapon Config")
	TMap<FGameplayTag, float> MaterialDamageMultiplier;

private:
	// Time since this weapon was last fired (relative to world time)
	// 上次射击的时间（相对于世界时间）
	double LastFireTime = 0.0;

	// The current heat
	// 当前热量
	float CurrentHeat = 0.0f;

	// The current spread angle (in degrees, diametrical)
	// 当前扩散角度（以度为单位，直径角）
	float CurrentSpreadAngle = 0.0f;

	// Do we currently have first shot accuracy?
	// 当前是否具有首发精准度
	bool bHasFirstShotAccuracy = false;

	// The current *combined* spread angle multiplier
	// 当前的扩散角度乘数
	float CurrentSpreadAngleMultiplier = 1.0f;

	// The current standing still multiplier
	// 当前的跳跃/下落乘数
	float StandingStillMultiplier = 1.0f;

	// The current jumping/falling multiplier
	// 当前的蹲下乘数
	float JumpFallMultiplier = 1.0f;

	// The current crouching multiplier
	// 当前的蹲下乘数
	float CrouchingMultiplier = 1.0f;

public:
	void Tick(float DeltaSeconds);

	//~USkyraEquipmentInstance interface
	virtual void OnEquipped();
	virtual void OnUnequipped();
	//~End of USkyraEquipmentInstance interface

	void AddSpread();

	//~ISkyraAbilitySourceInterface interface
	virtual float GetDistanceAttenuation(float Distance, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr) const override;
	virtual float GetPhysicalMaterialAttenuation(const UPhysicalMaterial* PhysicalMaterial, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr) const override;
	//~End of ISkyraAbilitySourceInterface interface

private:
	// 计算扩散范围
	void ComputeSpreadRange(float& MinSpread, float& MaxSpread);
	// 计算热量范围
	void ComputeHeatRange(float& MinHeat, float& MaxHeat);
	
	// 限制热量值
	inline float ClampHeat(float NewHeat)
	{
		float MinHeat;
		float MaxHeat;
		ComputeHeatRange(/*out*/ MinHeat, /*out*/ MaxHeat);

		return FMath::Clamp(NewHeat, MinHeat, MaxHeat);
	}

	// Updates the spread and returns true if the spread is at minimum
	// 更新扩散，返回是否达到最小值
	bool UpdateSpread(float DeltaSeconds);

	// Updates the multipliers and returns true if they are at minimum
	// 更新乘数，返回是否达到最小值
	bool UpdateMultipliers(float DeltaSeconds);
};
