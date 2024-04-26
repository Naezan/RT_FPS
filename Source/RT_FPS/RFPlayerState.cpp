// Fill out your copyright notice in the Description page of Project Settings.


#include "RFPlayerState.h"
#include "RFPlayerData.h"
#include "AbilitySystem/RFAbilitySystemComponent.h"
#include "RFLogMacros.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

ARFPlayerState::ARFPlayerState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilitySystemComponent = ObjectInitializer.CreateDefaultSubobject<URFAbilitySystemComponent>(this, TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	PlayerTeamID = FGenericTeamId::NoTeam;
}

void ARFPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

UAbilitySystemComponent* ARFPlayerState::GetAbilitySystemComponent() const
{
	return GetRFAbilitySystemComponent();
}

void ARFPlayerState::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	if (HasAuthority())
	{
		const FGenericTeamId OldTeamID = PlayerTeamID;
		PlayerTeamID = NewTeamID;
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PlayerTeamID, this);
	}
}

FGenericTeamId ARFPlayerState::GetGenericTeamId() const
{
	return PlayerTeamID;
}

void ARFPlayerState::SetPlayerData(const URFPlayerData* InPawnData)
{
	if (!HasAuthority())
	{
		return;
	}

	if (PlayerData)
	{
		UE_LOG(LogRF, Warning, TEXT("Already has PlayerData [%s]"), *GetNameSafe(PlayerData));
	}

	PlayerData = InPawnData;
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, PlayerData, this);

	ForceNetUpdate();
}

void ARFPlayerState::OnRep_PlayerTeamID(FGenericTeamId OldTeamID)
{
}

void ARFPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	SharedParams.RepNotifyCondition = ELifetimeRepNotifyCondition::REPNOTIFY_OnChanged;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PlayerData, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, PlayerTeamID, SharedParams);
}