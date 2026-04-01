// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayEffectTypes.h"

#include "SkyraGameplayEffectContext.generated.h"

class AActor;
class FArchive;
class ISkyraAbilitySourceInterface;
class UObject;
class UPhysicalMaterial;

USTRUCT()
struct FSkyraGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

	FSkyraGameplayEffectContext()
		: FGameplayEffectContext()
	{
	}

	FSkyraGameplayEffectContext(AActor* InInstigator, AActor* InEffectCauser)
		: FGameplayEffectContext(InInstigator, InEffectCauser)
	{
	}

	/** Returns the wrapped FSkyraGameplayEffectContext from the handle, or nullptr if it doesn't exist or is the wrong type */
	static SKYRAGAME_API FSkyraGameplayEffectContext* ExtractEffectContext(struct FGameplayEffectContextHandle Handle);

	/** Sets the object used as the ability source */
	void SetAbilitySource(const ISkyraAbilitySourceInterface* InObject, float InSourceLevel);

	/** Returns the ability source interface associated with the source object. Only valid on the authority. */
	const ISkyraAbilitySourceInterface* GetAbilitySource() const;

	virtual FGameplayEffectContext* Duplicate() const override
	{
		FSkyraGameplayEffectContext* NewContext = new FSkyraGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	virtual UScriptStruct* GetScriptStruct() const override
	{
		return FSkyraGameplayEffectContext::StaticStruct();
	}

	/** Overridden to serialize new fields */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override;

	/** Returns the physical material from the hit result if there is one */
	const UPhysicalMaterial* GetPhysicalMaterial() const;

public:
	/** ID to allow the identification of multiple bullets that were part of the same cartridge */
	UPROPERTY()
	int32 CartridgeID = -1;

protected:
	/** Ability Source object (should implement ISkyraAbilitySourceInterface). NOT replicated currently */
	UPROPERTY()
	TWeakObjectPtr<const UObject> AbilitySourceObject;
};

template<>
struct TStructOpsTypeTraits<FSkyraGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FSkyraGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};

