// Copyright Epic Games, Inc. All Rights Reserved.

#include "SkyraTeamCreationComponent.h"
#include "GameModes/SkyraExperienceManagerComponent.h"
#include "SkyraTeamPublicInfo.h"
#include "SkyraTeamPrivateInfo.h"
#include "Player/SkyraPlayerState.h"
#include "Engine/World.h"
#include "GameModes/SkyraGameMode.h"

#if WITH_EDITOR
#include "Misc/DataValidation.h"
#endif

#include UE_INLINE_GENERATED_CPP_BY_NAME(SkyraTeamCreationComponent)

USkyraTeamCreationComponent::USkyraTeamCreationComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PublicTeamInfoClass = ASkyraTeamPublicInfo::StaticClass();
	PrivateTeamInfoClass = ASkyraTeamPrivateInfo::StaticClass();
}

#if WITH_EDITOR
EDataValidationResult USkyraTeamCreationComponent::IsDataValid(FDataValidationContext& Context) const
{
	EDataValidationResult Result = Super::IsDataValid(Context);

	//@TODO: TEAMS: Validate that all display assets have the same properties set!

	return Result;
}
#endif

void USkyraTeamCreationComponent::BeginPlay()
{
	Super::BeginPlay();

	// Listen for the experience load to complete
	AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
	USkyraExperienceManagerComponent* ExperienceComponent = GameState->FindComponentByClass<USkyraExperienceManagerComponent>();
	check(ExperienceComponent);
	ExperienceComponent->CallOrRegister_OnExperienceLoaded_HighPriority(FOnSkyraExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void USkyraTeamCreationComponent::OnExperienceLoaded(const USkyraExperienceDefinition* Experience)
{
#if WITH_SERVER_CODE
	if (HasAuthority())
	{
		ServerCreateTeams();
		ServerAssignPlayersToTeams();
	}
#endif
}

#if WITH_SERVER_CODE

void USkyraTeamCreationComponent::ServerCreateTeams()
{
	for (const auto& KVP : TeamsToCreate)
	{
		const int32 TeamId = KVP.Key;
		ServerCreateTeam(TeamId, KVP.Value);
	}
}

void USkyraTeamCreationComponent::ServerAssignPlayersToTeams()
{
	// Assign players that already exist to teams
	AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
	for (APlayerState* PS : GameState->PlayerArray)
	{
		if (ASkyraPlayerState* SkyraPS = Cast<ASkyraPlayerState>(PS))
		{
			ServerChooseTeamForPlayer(SkyraPS);
		}
	}

	// Listen for new players logging in
	ASkyraGameMode* GameMode = Cast<ASkyraGameMode>(GameState->AuthorityGameMode);
	check(GameMode);

	GameMode->OnGameModePlayerInitialized.AddUObject(this, &ThisClass::OnPlayerInitialized);
}

void USkyraTeamCreationComponent::ServerChooseTeamForPlayer(ASkyraPlayerState* PS)
{
	if (PS->IsOnlyASpectator())
	{
		PS->SetGenericTeamId(FGenericTeamId::NoTeam);
	}
	else
	{
		const FGenericTeamId TeamID = IntegerToGenericTeamId(GetLeastPopulatedTeamID());
		PS->SetGenericTeamId(TeamID);
	}
}

void USkyraTeamCreationComponent::OnPlayerInitialized(AGameModeBase* GameMode, AController* NewPlayer)
{
	check(NewPlayer);
	check(NewPlayer->PlayerState);
	if (ASkyraPlayerState* SkyraPS = Cast<ASkyraPlayerState>(NewPlayer->PlayerState))
	{
		ServerChooseTeamForPlayer(SkyraPS);
	}
}

void USkyraTeamCreationComponent::ServerCreateTeam(int32 TeamId, USkyraTeamDisplayAsset* DisplayAsset)
{
	check(HasAuthority());

	//@TODO: ensure the team doesn't already exist

	UWorld* World = GetWorld();
	check(World);

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ASkyraTeamPublicInfo* NewTeamPublicInfo = World->SpawnActor<ASkyraTeamPublicInfo>(PublicTeamInfoClass, SpawnInfo);
	checkf(NewTeamPublicInfo != nullptr, TEXT("Failed to create public team actor from class %s"), *GetPathNameSafe(*PublicTeamInfoClass));
	NewTeamPublicInfo->SetTeamId(TeamId);
	NewTeamPublicInfo->SetTeamDisplayAsset(DisplayAsset);

	ASkyraTeamPrivateInfo* NewTeamPrivateInfo = World->SpawnActor<ASkyraTeamPrivateInfo>(PrivateTeamInfoClass, SpawnInfo);
	checkf(NewTeamPrivateInfo != nullptr, TEXT("Failed to create private team actor from class %s"), *GetPathNameSafe(*PrivateTeamInfoClass));
	NewTeamPrivateInfo->SetTeamId(TeamId);
}

int32 USkyraTeamCreationComponent::GetLeastPopulatedTeamID() const
{
	const int32 NumTeams = TeamsToCreate.Num();
	if (NumTeams > 0)
	{
		TMap<int32, uint32> TeamMemberCounts;
		TeamMemberCounts.Reserve(NumTeams);

		for (const auto& KVP : TeamsToCreate)
		{
			const int32 TeamId = KVP.Key;
			TeamMemberCounts.Add(TeamId, 0);
		}

		AGameStateBase* GameState = GetGameStateChecked<AGameStateBase>();
		for (APlayerState* PS : GameState->PlayerArray)
		{
			if (ASkyraPlayerState* SkyraPS = Cast<ASkyraPlayerState>(PS))
			{
				const int32 PlayerTeamID = SkyraPS->GetTeamId();

				if ((PlayerTeamID != INDEX_NONE) && !SkyraPS->IsInactive())	// do not count unassigned or disconnected players
				{
					check(TeamMemberCounts.Contains(PlayerTeamID))
					TeamMemberCounts[PlayerTeamID] += 1;
				}
			}
		}

		// sort by lowest team population, then by team ID
		int32 BestTeamId = INDEX_NONE;
		uint32 BestPlayerCount = TNumericLimits<uint32>::Max();
		for (const auto& KVP : TeamMemberCounts)
		{
			const int32 TestTeamId = KVP.Key;
			const uint32 TestTeamPlayerCount = KVP.Value;

			if (TestTeamPlayerCount < BestPlayerCount)
			{
				BestTeamId = TestTeamId;
				BestPlayerCount = TestTeamPlayerCount;
			}
			else if (TestTeamPlayerCount == BestPlayerCount)
			{
				if ((TestTeamId < BestTeamId) || (BestTeamId == INDEX_NONE))
				{
					BestTeamId = TestTeamId;
					BestPlayerCount = TestTeamPlayerCount;
				}
			}
		}

		return BestTeamId;
	}

	return INDEX_NONE;
}
#endif	// WITH_SERVER_CODE

