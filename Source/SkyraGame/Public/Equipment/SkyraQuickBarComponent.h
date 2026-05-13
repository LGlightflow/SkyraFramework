// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ControllerComponent.h"
#include "Inventory/SkyraInventoryItemInstance.h"

#include "SkyraQuickBarComponent.generated.h"

class AActor;
class USkyraEquipmentInstance;
class USkyraEquipmentManagerComponent;
class UObject;
struct FFrame;

/*
 *快捷栏 / 武器栏 / 当前装备管理器
 * 负责内容
 *	保存当前拥有的快捷栏物品
	切换当前激活槽位
	装备/卸下武器
	同步到客户端
	广播 UI 消息
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class USkyraQuickBarComponent : public UControllerComponent
{
	GENERATED_BODY()

public:
	USkyraQuickBarComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="Skyra")
	void CycleActiveSlotForward();

	UFUNCTION(BlueprintCallable, Category="Skyra")
	void CycleActiveSlotBackward();

	UFUNCTION(Server, Reliable, BlueprintCallable, Category="Skyra")
	void SetActiveSlotIndex(int32 NewIndex);

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	TArray<USkyraInventoryItemInstance*> GetSlots() const
	{
		return Slots;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	int32 GetActiveSlotIndex() const { return ActiveSlotIndex; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false)
	USkyraInventoryItemInstance* GetActiveSlotItem() const;

	UFUNCTION(BlueprintCallable, BlueprintPure=false)
	int32 GetNextFreeItemSlot() const;

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void AddItemToSlot(int32 SlotIndex, USkyraInventoryItemInstance* Item);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	USkyraInventoryItemInstance* RemoveItemFromSlot(int32 SlotIndex);

	virtual void BeginPlay() override;

private:
	void UnequipItemInSlot();
	void EquipItemInSlot();

	USkyraEquipmentManagerComponent* FindEquipmentManager() const;

protected:
	UPROPERTY()
	int32 NumSlots = 3;

	UFUNCTION()
	void OnRep_Slots();

	UFUNCTION()
	void OnRep_ActiveSlotIndex();

private:
	//拥有装备列表
	UPROPERTY(ReplicatedUsing=OnRep_Slots)
	TArray<TObjectPtr<USkyraInventoryItemInstance>> Slots;
	
	//当前装备的索引
	UPROPERTY(ReplicatedUsing=OnRep_ActiveSlotIndex)
	int32 ActiveSlotIndex = -1;
	
	//当前装备
	UPROPERTY()
	TObjectPtr<USkyraEquipmentInstance> EquippedItem;
};


USTRUCT(BlueprintType)
struct FSkyraQuickBarSlotsChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TArray<TObjectPtr<USkyraInventoryItemInstance>> Slots;
};


USTRUCT(BlueprintType)
struct FSkyraQuickBarActiveIndexChangedMessage
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<AActor> Owner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 ActiveIndex = 0;
};
