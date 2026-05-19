// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ControllerComponent.h"
#include "GameplayTagContainer.h"

#include "SkyraWeaponStateComponent.generated.h"

class UObject;
struct FFrame;
struct FGameplayAbilityTargetDataHandle;
struct FGameplayEffectContextHandle;
struct FHitResult;

// Hit markers are shown for ranged weapon impacts in the reticle
// A 'successful' hit marker is shown for impacts that damaged an enemy
/**
 * 单个命中点在屏幕空间中的信息
 * 用于准星 HitMarker 显示
 */
struct FSkyraScreenSpaceHitLocation
{
	/** 命中的屏幕坐标 Hit location in viewport screenspace */
	FVector2D Location;	
	/** 命中区域标签，例如 Gameplay.Zone.Head */
	FGameplayTag HitZone;
	/** 是否是有效命中（命中敌人） */
	bool bShowAsSuccess = false;
};

/**
 * 一次射击对应的一批命中数据
 *
 * 因为客户端本地预测会先显示命中，
 * 后续服务端确认后才真正显示“有效命中”
 */
struct FSkyraServerSideHitMarkerBatch
{
	FSkyraServerSideHitMarkerBatch() { }

	FSkyraServerSideHitMarkerBatch(uint8 InUniqueId) :
		UniqueId(InUniqueId)
	{ }

	TArray<FSkyraScreenSpaceHitLocation> Markers;

	uint8 UniqueId = 0;
};

// Tracks weapon state and recent confirmed hit markers to display on screen
/**
 * 武器状态组件
 *
 * 挂在 Controller 上
 * 主要负责：
 * 1. 更新当前武器 Tick
 * 2. 管理客户端预测命中
 * 3. 管理服务端确认命中
 * 4. 记录屏幕 HitMarker
 * 5. 提供 HUD 命中反馈
 */

UCLASS()
class USkyraWeaponStateComponent : public UControllerComponent
{
	GENERATED_BODY()

public:

	USkyraWeaponStateComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * 服务端确认命中结果
	 *
	 * Client RPC：
	 * 服务端验证 TargetData 后回调客户端
	 *
	 * 参数：
	 * UniqueId     -> 对应哪一次射击
	 * bSuccess     -> 是否命中成功
	 * HitReplaces  -> 被服务端替换掉的命中点
	 */
	UFUNCTION(Client, Reliable)
	void ClientConfirmTargetData(uint16 UniqueId, bool bSuccess, const TArray<uint8>& HitReplaces);
	
	void AddUnconfirmedServerSideHitMarkers(const FGameplayAbilityTargetDataHandle& InTargetData, const TArray<FHitResult>& FoundHits);
	
	
	/** Updates this player's last damage instigated time */
	/**
	 * 更新最近一次造成伤害的时间
	 *
	 * 用于 HUD HitMarker 闪烁
	 */
	void UpdateDamageInstigatedTime(const FGameplayEffectContextHandle& EffectContext);

	/** Gets the array of most recent locations this player instigated damage, in screen-space */
	/**
	 * 获取最近一次命中的屏幕坐标
	 * HUD 可以读取这个数据绘制 HitMarker
	 */
	void GetLastWeaponDamageScreenLocations(TArray<FSkyraScreenSpaceHitLocation>& WeaponDamageScreenLocations)
	{
		WeaponDamageScreenLocations = LastWeaponDamageScreenLocations;
	}

	/** Returns the elapsed time since the last (outgoing) damage hit notification occurred */
	/**
	 * 距离上次命中已经过去多久
	 */
	double GetTimeSinceLastHitNotification() const;
	
	/**
	 * 当前还有多少未确认命中
	 */
	int32 GetUnconfirmedServerSideHitMarkerCount() const
	{
		return UnconfirmedServerSideHitMarkers.Num();
	}

protected:
	// This is called to filter hit results to determine whether they should be considered as a successful hit or not
	// The default behavior is to treat it as a success if being done to a team actor that belongs to a different team
	// to the owning controller's pawn
	
	/**
	 * 判断这个命中是否算“有效命中”
	 *
	 * 默认逻辑：
	 * - 只有敌对阵营才算成功命中
	 */
	virtual bool ShouldShowHitAsSuccess(const FHitResult& Hit) const;
	
	/**
	 * 是否应该更新“造成伤害时间”
	 *
	 * 默认：
	 * 只要有 EffectCauser 就更新
	 */
	virtual bool ShouldUpdateDamageInstigatedTime(const FGameplayEffectContextHandle& EffectContext) const;
	
	/**
	 * 真正更新命中时间
	 */
	void ActuallyUpdateDamageInstigatedTime();

private:
	/** Last time this controller instigated weapon damage */
	/**
	 * 最近一次造成武器伤害的时间
	 *
	 * 用于：
	 * - HUD HitMarker 动画
	 * - 准星反馈
	 */
	double LastWeaponDamageInstigatedTime = 0.0;

	/** Screen-space locations of our most recently instigated weapon damage (the confirmed hits) */
	/**
	 * 最近一次已确认命中的屏幕坐标
	 *
	 * HUD 每帧读取这里绘制命中反馈
	 */
	TArray<FSkyraScreenSpaceHitLocation> LastWeaponDamageScreenLocations;

	/** The unconfirmed hits */
	/**
	 * 客户端预测的“未确认命中”
	 *
	 * 客户端先本地预测：
	 * - 播放 HitMarker
	 * - 等待服务端确认
	 *
	 * 服务端确认后：
	 * - 转移到 LastWeaponDamageScreenLocations
	 * - 或直接丢弃
	 */
	TArray<FSkyraServerSideHitMarkerBatch> UnconfirmedServerSideHitMarkers;
};
