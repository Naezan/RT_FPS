// Fill out your copyright notice in the Description page of Project Settings.


#include "RecoilManagerComponent.h"
#include "RecoilPatternAsset.h"
#include "RecoilGrid.h"
#include "Net/UnrealNetwork.h"

URecoilManagerComponent::URecoilManagerComponent()
{
}

void URecoilManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(URecoilManagerComponent, RepliactedRecoilRotation);
}

void URecoilManagerComponent::TriggerRecoil()
{
	if (RecoilPatternAsset == nullptr)
	{
		return;
	}

	FRotator RecoilRotation = GetRecoilDeltaRotation(RecoilIndex);

	const FRotator CurControlRotation = GetControlRotation().GetNormalized();

	RecoilSpeed = FMath::FInterpConstantTo(RecoilSpeed, RecoilPatternAsset->RecoilInterpParams.MaxRecoilSpeed, GetWorld()->GetDeltaSeconds(), RecoilPatternAsset->RecoilInterpParams.RecoilInterpSpeed);
	FRotator TargetControlRotation = FMath::QInterpConstantTo(CurControlRotation.Quaternion(), TargetControlRotation.Quaternion(), GetWorld()->GetDeltaSeconds(), RecoilSpeed).Rotator().GetNormalized();
	TargetControlRotation.Roll = 0.f;

	const FRotator TargetDeltaRotation = (TargetControlRotation - CurControlRotation).GetNormalized();

	RepliactedRecoilRotation = CurControlRotation + TargetDeltaRotation;
	SetControlRotation(CurControlRotation + TargetDeltaRotation);

	++RecoilIndex;
}

void URecoilManagerComponent::CompleteRecoil()
{
	const int32 PointsNum = RecoilPatternAsset->GetRecoilPointsNum();
	RecoilIndex %= PointsNum;

	RecoilSpeed = 0.f;
}

AController* URecoilManagerComponent::GetOwningController() const
{
	AController* OwningController = nullptr;

	AActor* TargetOwner = GetOwner();
	while (!OwningController && TargetOwner)
	{
		OwningController = Cast<AController>(TargetOwner);
		TargetOwner = TargetOwner->GetOwner();
	}

	return OwningController;
}

FRotator URecoilManagerComponent::GetControlRotation() const
{
	const AController* OwningController = GetOwningController();

	return OwningController ? OwningController->GetControlRotation() : FRotator::ZeroRotator;
}

void URecoilManagerComponent::SetControlRotation_Implementation(const FRotator& Rotation)
{
	if (AController* OwningController = GetOwningController())
	{
		OwningController->SetControlRotation(Rotation.GetDenormalized());
	}
}

URecoilPatternAsset* URecoilManagerComponent::GetRecoilPattern() const
{
	return RecoilPatternAsset;
}

FRotator URecoilManagerComponent::GetRecoilDeltaRotation(int32 InRecoilIndex) const
{
	if (RecoilPatternAsset == nullptr)
	{
		return FRotator::ZeroRotator;
	}

	const int32 PointsNum = RecoilPatternAsset->GetRecoilPointsNum();

	if (PointsNum <= InRecoilIndex)
	{
		InRecoilIndex %= PointsNum;
	}

	const FRecoilPoint CurRecoilPoint = RecoilPatternAsset->GetRecoilPointAt(InRecoilIndex);
	const FRecoilPoint PrevRecoilPoint = InRecoilIndex > 0 ? RecoilPatternAsset->GetRecoilPointAt(InRecoilIndex) : FRecoilPoint();
	
	const FVector2D DeltaRecoilRotation = CurRecoilPoint.PointCoord - PrevRecoilPoint.PointCoord;

	return FRotator(DeltaRecoilRotation.Y, DeltaRecoilRotation.X, 0);
}


// Called when the game starts
void URecoilManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	SetIsReplicated(true);
}
