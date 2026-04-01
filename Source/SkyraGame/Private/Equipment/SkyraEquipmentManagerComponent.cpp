// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkyraEquipmentManagerComponent.h"

#include "AbilitySystem/SkyraAbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Engine/ActorChannel.h"
#include "SkyraEquipmentDefinition.h"
#include "SkyraEquipmentInstance.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SkyraEquipmentManagerComponent)

class FLifetimeProperty;
struct FReplicationFlags;

//////////////////////////////////////////////////////////////////////
// FSkyraAppliedEquipmentEntry

FString FSkyraAppliedEquipmentEntry::GetDebugString() const
{
	return FString::Printf(TEXT("%s of %s"), *GetNameSafe(Instance), *GetNameSafe(EquipmentDefinition.Get()));
}

//////////////////////////////////////////////////////////////////////
// FSkyraEquipmentList

void FSkyraEquipmentList::PreReplicatedRemove(const TArrayView<int32> RemovedIndices, int32 FinalSize)
{
 	for (int32 Index : RemovedIndices)
 	{
 		const FSkyraAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnUnequipped();
		}
 	}
}

void FSkyraEquipmentList::PostReplicatedAdd(const TArrayView<int32> AddedIndices, int32 FinalSize)
{
	for (int32 Index : AddedIndices)
	{
		const FSkyraAppliedEquipmentEntry& Entry = Entries[Index];
		if (Entry.Instance != nullptr)
		{
			Entry.Instance->OnEquipped();
		}
	}
}

void FSkyraEquipmentList::PostReplicatedChange(const TArrayView<int32> ChangedIndices, int32 FinalSize)
{
// 	for (int32 Index : ChangedIndices)
// 	{
// 		const FGameplayTagStack& Stack = Stacks[Index];
// 		TagToCountMap[Stack.Tag] = Stack.StackCount;
// 	}
}

USkyraAbilitySystemComponent* FSkyraEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	return Cast<USkyraAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

USkyraEquipmentInstance* FSkyraEquipmentList::AddEntry(TSubclassOf<USkyraEquipmentDefinition> EquipmentDefinition)
{
	USkyraEquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
 	check(OwnerComponent);
	check(OwnerComponent->GetOwner()->HasAuthority());
	
	const USkyraEquipmentDefinition* EquipmentCDO = GetDefault<USkyraEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<USkyraEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = USkyraEquipmentInstance::StaticClass();
	}
	
	FSkyraAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<USkyraEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);  //@TODO: Using the actor instead of component as the outer due to UE-127172
	Result = NewEntry.Instance;

	if (USkyraAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (const TObjectPtr<const USkyraAbilitySet>& AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, /*inout*/ &NewEntry.GrantedHandles, Result);
		}
	}
	else
	{
		//@TODO: Warning logging?
	}

	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);


	MarkItemDirty(NewEntry);

	return Result;
}

void FSkyraEquipmentList::RemoveEntry(USkyraEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FSkyraAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			if (USkyraAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			Instance->DestroyEquipmentActors();
			

			EntryIt.RemoveCurrent();
			MarkArrayDirty();
		}
	}
}

//////////////////////////////////////////////////////////////////////
// USkyraEquipmentManagerComponent

USkyraEquipmentManagerComponent::USkyraEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
	SetIsReplicatedByDefault(true);
	bWantsInitializeComponent = true;
}

void USkyraEquipmentManagerComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, EquipmentList);
}

USkyraEquipmentInstance* USkyraEquipmentManagerComponent::EquipItem(TSubclassOf<USkyraEquipmentDefinition> EquipmentClass)
{
	USkyraEquipmentInstance* Result = nullptr;
	if (EquipmentClass != nullptr)
	{
		Result = EquipmentList.AddEntry(EquipmentClass);
		if (Result != nullptr)
		{
			Result->OnEquipped();

			if (IsUsingRegisteredSubObjectList() && IsReadyForReplication())
			{
				AddReplicatedSubObject(Result);
			}
		}
	}
	return Result;
}

void USkyraEquipmentManagerComponent::UnequipItem(USkyraEquipmentInstance* ItemInstance)
{
	if (ItemInstance != nullptr)
	{
		if (IsUsingRegisteredSubObjectList())
		{
			RemoveReplicatedSubObject(ItemInstance);
		}

		ItemInstance->OnUnequipped();
		EquipmentList.RemoveEntry(ItemInstance);
	}
}

bool USkyraEquipmentManagerComponent::ReplicateSubobjects(UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);

	for (FSkyraAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		USkyraEquipmentInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			WroteSomething |= Channel->ReplicateSubobject(Instance, *Bunch, *RepFlags);
		}
	}

	return WroteSomething;
}

void USkyraEquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void USkyraEquipmentManagerComponent::UninitializeComponent()
{
	TArray<USkyraEquipmentInstance*> AllEquipmentInstances;

	// gathering all instances before removal to avoid side effects affecting the equipment list iterator	
	for (const FSkyraAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		AllEquipmentInstances.Add(Entry.Instance);
	}

	for (USkyraEquipmentInstance* EquipInstance : AllEquipmentInstances)
	{
		UnequipItem(EquipInstance);
	}

	Super::UninitializeComponent();
}

void USkyraEquipmentManagerComponent::ReadyForReplication()
{
	Super::ReadyForReplication();

	// Register existing SkyraEquipmentInstances
	if (IsUsingRegisteredSubObjectList())
	{
		for (const FSkyraAppliedEquipmentEntry& Entry : EquipmentList.Entries)
		{
			USkyraEquipmentInstance* Instance = Entry.Instance;

			if (IsValid(Instance))
			{
				AddReplicatedSubObject(Instance);
			}
		}
	}
}

USkyraEquipmentInstance* USkyraEquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<USkyraEquipmentInstance> InstanceType)
{
	for (FSkyraAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (USkyraEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

TArray<USkyraEquipmentInstance*> USkyraEquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<USkyraEquipmentInstance> InstanceType) const
{
	TArray<USkyraEquipmentInstance*> Results;
	for (const FSkyraAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (USkyraEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}


