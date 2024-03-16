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
}

void ARFWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	SharedParams.RepNotifyCondition = ELifetimeRepNotifyCondition::REPNOTIFY_OnChanged;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, IsFPWeapon, SharedParams);
}
