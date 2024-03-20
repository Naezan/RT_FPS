// Fill out your copyright notice in the Description page of Project Settings.


#include "RFWeaponBase.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// Sets default values
ARFWeaponBase::ARFWeaponBase()
{
	bReplicates = true;
}

void ARFWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARFWeaponBase::SetFPAttribute(bool IsFP)
{
	IsFPWeapon = IsFP;
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, IsFPWeapon, this);
}

void ARFWeaponBase::SetAttachedMagActor(AActor* InMagActor)
{
	AttachedMagActor = InMagActor;
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, IsFPWeapon, this);
}

void ARFWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	SharedParams.RepNotifyCondition = ELifetimeRepNotifyCondition::REPNOTIFY_OnChanged;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, IsFPWeapon, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, AttachedMagActor, SharedParams);
}
