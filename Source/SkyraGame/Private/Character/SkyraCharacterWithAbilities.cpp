// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkyraCharacterWithAbilities.h"

#include "SkyraAbilitySet.h"
#include "SkyraLogChannels.h"
#include "SkyraPawnExtensionComponent.h"
#include "AbilitySystem/Attributes/SkyraCombatSet.h"
#include "AbilitySystem/Attributes/SkyraHealthSet.h"
#include "AbilitySystem/SkyraAbilitySystemComponent.h"
#include "Async/TaskGraphInterfaces.h"
#include "Character/SkyraPawnData.h"
#include "Components/GameFrameworkComponentManager.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SkyraCharacterWithAbilities)

const FName ASkyraCharacterWithAbilities::NAME_CharacterWithSkyraAbilityReady("SkyraAbilitiesReady");

ASkyraCharacterWithAbilities::ASkyraCharacterWithAbilities(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<USkyraAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// These attribute sets will be detected by AbilitySystemComponent::InitializeComponent. Keeping a reference so that the sets don't get garbage collected before that.
	HealthSet = CreateDefaultSubobject<USkyraHealthSet>(TEXT("HealthSet"));
	CombatSet = CreateDefaultSubobject<USkyraCombatSet>(TEXT("CombatSet"));

	// AbilitySystemComponent needs to be updated at a high frequency.
	NetUpdateFrequency = 100.0f;
}

void ASkyraCharacterWithAbilities::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	check(AbilitySystemComponent);
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}

UAbilitySystemComponent* ASkyraCharacterWithAbilities::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ASkyraCharacterWithAbilities::BeginPlay()
{
	Super::BeginPlay();

		
	if (CustomDefaultPawnData)
	{
		SetPawnData(CustomDefaultPawnData);
		PawnExtComponent->SetPawnData(CustomDefaultPawnData);
	}
	if (PawnExtComponent)
	{
		
		if (AbilitySystemComponent)
		{
			AbilitySystemComponent->ClearActorInfo();
		}
		
		PawnExtComponent->InitializeAbilitySystem(this->AbilitySystemComponent, this);
		
		
		

	}
}

void ASkyraCharacterWithAbilities::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void ASkyraCharacterWithAbilities::SetPawnData(const USkyraPawnData* InPawnData)
{
	check(InPawnData);

	if (GetLocalRole() != ROLE_Authority)
	{
		return;
	}
	for (const USkyraAbilitySet* AbilitySet : InPawnData->AbilitySets)
	{
		if (AbilitySet)
		{
			AbilitySet->GiveToAbilitySystem(AbilitySystemComponent, nullptr);
		}
	}

	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(this, NAME_CharacterWithSkyraAbilityReady);
	
	ForceNetUpdate();
}
