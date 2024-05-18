// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/RFGameplayAbility_Death.h"
#include "AbilitySystemComponent.h"
#include "Player/RFCharacter.h"

URFGameplayAbility_Death::URFGameplayAbility_Death(const FObjectInitializer& ObjectInitializer)
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerOnly;
}

void URFGameplayAbility_Death::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	// Cancel all abilities
	ASC->CancelAbilities(nullptr, nullptr, this);
	// Block Cancel
	SetCanBeCanceled(false);

	// Calling before blueprint event triggered
	DeathStart();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void URFGameplayAbility_Death::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	// Calling before blueprint event triggered
	DeathFinish();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URFGameplayAbility_Death::DeathStart()
{
	if (ARFCharacter* AvatarActor = Cast<ARFCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (AvatarActor->GetDeathStatus() == EDeathState::Alive)
		{
			AvatarActor->DeathStart();
		}
	}
}

void URFGameplayAbility_Death::DeathFinish()
{
	if (ARFCharacter* AvatarActor = Cast<ARFCharacter>(GetAvatarActorFromActorInfo()))
	{
		if (AvatarActor->GetDeathStatus() == EDeathState::DeathStarted)
		{
			AvatarActor->DeathFinish();
		}
	}
}
