// Fill out your copyright notice in the Description page of Project Settings.


#include "RFMagazineBase.h"
#include "RFLogMacros.h"
#include "Net/UnrealNetwork.h"
#include "Net/Core/PushModel/PushModel.h"

// Sets default values
ARFMagazineBase::ARFMagazineBase()
{
	bReplicates = true;

	MagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagMeshComp"));
	MagMesh->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void ARFMagazineBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARFMagazineBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	SharedParams.RepNotifyCondition = ELifetimeRepNotifyCondition::REPNOTIFY_OnChanged;

	DOREPLIFETIME_WITH_PARAMS_FAST(ThisClass, MagMesh, SharedParams);
}

void ARFMagazineBase::OnAllAmmoConsumed()
{
	if (EmptyMagMesh && MagMesh)
	{
		MagMesh->SetStaticMesh(EmptyMagMesh);
		MARK_PROPERTY_DIRTY_FROM_NAME(ThisClass, MagMesh, this);
	}
}

