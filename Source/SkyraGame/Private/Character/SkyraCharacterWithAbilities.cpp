// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkyraCharacterWithAbilities.h"

#include "SkyraPawnExtensionComponent.h"
#include "AbilitySystem/Attributes/SkyraCombatSet.h"
#include "AbilitySystem/Attributes/SkyraHealthSet.h"
#include "AbilitySystem/SkyraAbilitySystemComponent.h"
#include "Async/TaskGraphInterfaces.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(SkyraCharacterWithAbilities)

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
	
	if (PawnExtComponent)
	{
		PawnExtComponent->InitializeAbilitySystem(AbilitySystemComponent, this);
	}
}

UAbilitySystemComponent* ASkyraCharacterWithAbilities::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

