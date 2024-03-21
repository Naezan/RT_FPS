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

void ARFWeaponBase::SetFireSelectMode(EFireSelectMode InSelectMode)
{
	FireSelectMode = InSelectMode;
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, FireSelectMode, this);
}

EFireSelectMode ARFWeaponBase::UpSelectMode()
{
	switch (FireSelectMode)
	{
	case EFireSelectMode::Safety:
		FireSelectMode = EFireSelectMode::Semi;
		break;
	case EFireSelectMode::Semi:
		FireSelectMode = EFireSelectMode::Auto;
		break;
	}
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, FireSelectMode, this);

	return FireSelectMode;
}

EFireSelectMode ARFWeaponBase::DownSelectMode()
{
	switch (FireSelectMode)
	{
	case EFireSelectMode::Semi:
		FireSelectMode = EFireSelectMode::Safety;
		break;
	case EFireSelectMode::Auto:
		FireSelectMode = EFireSelectMode::Semi;
		break;
	}
	MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, FireSelectMode, this);

	return FireSelectMode;
}

void ARFWeaponBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	SharedParams.RepNotifyCondition = ELifetimeRepNotifyCondition::REPNOTIFY_OnChanged;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, IsFPWeapon, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, AttachedMagActor, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, FireSelectMode, SharedParams);
}
