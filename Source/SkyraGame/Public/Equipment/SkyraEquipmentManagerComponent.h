// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AbilitySystem/SkyraAbilitySet.h"
#include "Components/PawnComponent.h"
#include "Net/Serialization/FastArraySerializer.h"

#include "SkyraEquipmentManagerComponent.generated.h"

class UActorComponent;
class USkyraAbilitySystemComponent;
class USkyraEquipmentDefinition;
class USkyraEquipmentInstance;
class USkyraEquipmentManagerComponent;
class UObject;
struct FFrame;
struct FSkyraEquipmentList;
struct FNetDeltaSerializeInfo;
struct FReplicationFlags;

/** A single piece of applied equipment */
USTRUCT(BlueprintType)
struct FSkyraAppliedEquipmentEntry : public FFastArraySerializerItem
{
	GENERATED_BODY()

	FSkyraAppliedEquipmentEntry()
	{}

	FString GetDebugString() const;

private:
	friend FSkyraEquipmentList;
	friend USkyraEquipmentManagerComponent;

	// The equipment class that got equipped
	UPROPERTY()
	TSubclassOf<USkyraEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<USkyraEquipmentInstance> Instance = nullptr;

	// Authority-only list of granted handles
	UPROPERTY(NotReplicated)
	FSkyraAbilitySet_GrantedHandles GrantedHandles;
};

/** List of applied equipment */
USTRUCT(BlueprintType)
struct FSkyraEquipmentList : public FFastArraySerializer
{
	GENERATED_BODY()

	FSkyraEquipmentList()
		: OwnerComponent(nullptr)
	{
	}

	FSkyraEquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

public:
	//~FFastArraySerializer contract
	void PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize);
	//~End of FFastArraySerializer contract

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FSkyraAppliedEquipmentEntry, FSkyraEquipmentList>(Entries, DeltaParms, *this);
	}

	USkyraEquipmentInstance* AddEntry(TSubclassOf<USkyraEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(USkyraEquipmentInstance* Instance);

private:
	USkyraAbilitySystemComponent* GetAbilitySystemComponent() const;

	friend USkyraEquipmentManagerComponent;

private:
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FSkyraAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};

template<>
struct TStructOpsTypeTraits<FSkyraEquipmentList> : public TStructOpsTypeTraitsBase2<FSkyraEquipmentList>
{
	enum { WithNetDeltaSerializer = true };
};










/**
 * Manages equipment applied to a pawn
 */
UCLASS(BlueprintType, Const)
class USkyraEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	USkyraEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	USkyraEquipmentInstance* EquipItem(TSubclassOf<USkyraEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly)
	void UnequipItem(USkyraEquipmentInstance* ItemInstance);

	//~UObject interface
	virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//~End of UObject interface

	//~UActorComponent interface
	//virtual void EndPlay() override;
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	virtual void ReadyForReplication() override;
	//~End of UActorComponent interface

	/** Returns the first equipped instance of a given type, or nullptr if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	USkyraEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<USkyraEquipmentInstance> InstanceType);

 	/** Returns all equipped instances of a given type, or an empty array if none are found */
 	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<USkyraEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<USkyraEquipmentInstance> InstanceType) const;

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

private:
	UPROPERTY(Replicated)
	FSkyraEquipmentList EquipmentList;
};
