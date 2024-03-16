// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/RFGameplayAbility_Ranged.h"
#include "RFWeaponInstance.h"
#include "RFLogMacros.h"
#include "Collision\RFCollisionChannel.h"

// Reference by Lyra
namespace RFConsoleVariables
{
	static float DrawBulletHitDuration = 0.0f;
	static FAutoConsoleVariableRef CVarDrawBulletHitDuration(
		TEXT("RF.Weapon.DrawBulletHitDuration"),
		DrawBulletHitDuration,
		TEXT("Should we do debug drawing for bullet impacts (if above zero, sets how long (in seconds))"),
		ECVF_Default);

	static float DrawBulletTraceDuration = 0.0f;
	static FAutoConsoleVariableRef CVarDrawBulletTraceDuration(
		TEXT("RF.Weapon.DrawBulletTraceDuration"),
		DrawBulletTraceDuration,
		TEXT("Should we do debug drawing for bullet traces (if above zero, sets how long (in seconds))"),
		ECVF_Default);

	static float DrawBulletHitRadius = 3.0f;
	static FAutoConsoleVariableRef CVarDrawBulletHitRadius(
		TEXT("RF.Weapon.DrawBulletHitRadius"),
		DrawBulletHitRadius,
		TEXT("When bullet hit debug drawing is enabled (see DrawBulletHitDuration), how big should the hit radius be? (in uu)"),
		ECVF_Default);
}

void URFGameplayAbility_Ranged::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ClientTargetTrace();
}

void URFGameplayAbility_Ranged::ClientTargetTrace()
{
	if (!IsLocallyControlled())
	{
		UE_LOG(LogRF, Warning, TEXT("ClientTargetTrace : Server Cant call this function. [%s]'s Authority = [%d]"), *GetNameSafe(CurrentActorInfo->OwnerActor.Get()), CurrentActorInfo->IsNetAuthority());
	}

	TArray<FHitResult> FoundTargetHit;
	PerformBulletTrace(FoundTargetHit);

	if (FoundTargetHit.Num() > 0)
	{
		for (const FHitResult& FoundHit : FoundTargetHit)
		{
#if ENABLE_DRAW_DEBUG
			if (RFConsoleVariables::DrawBulletHitDuration > 0.0f)
			{
				DrawDebugPoint(GetWorld(), FoundHit.ImpactPoint, RFConsoleVariables::DrawBulletHitRadius, FColor::Red, false, RFConsoleVariables::DrawBulletHitRadius);
			}
#endif
		}
	}

	//SendTargetDataToServer();
}

void URFGameplayAbility_Ranged::PerformBulletTrace(OUT TArray<FHitResult>& OutHit)
{
	URFWeaponInstance* WeaponData = GetWeaponInstance();

	if (WeaponData == nullptr)
	{
		UE_LOG(LogRF, Error, TEXT("WeaponData is not valid [%s]"), *RF_CUR_CLASS_LINE);
		return;
	}

	float ActualSpreadHalfAngle = WeaponData->GetSpreadHalfAngle() * WeaponData->GetSpreadAngleMultiplier();
	float FireClusterScale = WeaponData->GetFireCluster();

	FVector TraceStart = WeaponData->GetMuzzleLocation();
	FVector CameraDir = GetCameraRotation().RotateVector(FVector::ForwardVector);
	FVector ActualRecoilDorection = ApplyRecoilToBulletDirection(CameraDir, ActualSpreadHalfAngle, FireClusterScale);
	FVector TraceEnd = TraceStart + ActualRecoilDorection * WeaponData->GetFireEffectiveRange();

#if ENABLE_DRAW_DEBUG
	if (RFConsoleVariables::DrawBulletTraceDuration > 0.0f)
	{
		static float DebugThickness = 1.0f;
		DrawDebugLine(GetWorld(), TraceStart, TraceStart + CameraDir * WeaponData->GetFireEffectiveRange(), FColor::Blue, false, RFConsoleVariables::DrawBulletTraceDuration, 0, DebugThickness);
	}
#endif // ENABLE_DRAW_DEBUG

	const int32 CartridgeCount = WeaponData->GetOneShotCartridge();
	const float SweepRadius = WeaponData->GetFireTraceRadius();
	BulletTraceByCartridge(OutHit, TraceStart, TraceEnd, CartridgeCount, SweepRadius);
}

void URFGameplayAbility_Ranged::BulletTraceByCartridge(OUT TArray<FHitResult>& OutHit, const FVector& TraceStart, const FVector& TraceEnd, int32 InCartridgeCount, float SweepRadius)
{
	for (int i = 0; i < InCartridgeCount; ++i)
	{
		// Do first SingleLineTrace for simple to found target
		FHitResult HitInfo = BulletTrace(OutHit, TraceStart, TraceEnd, 0.f);

#if ENABLE_DRAW_DEBUG
		if (RFConsoleVariables::DrawBulletTraceDuration > 0.0f)
		{
			static float DebugThickness = 1.0f;
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, RFConsoleVariables::DrawBulletTraceDuration, 0, DebugThickness);
		}
#endif // ENABLE_DRAW_DEBUG

		if (!HitInfo.bBlockingHit)
		{
			// Perform SweepLineTrace if none of the Hits have been detected
			FHitResult SweepHitInfo = BulletTrace(OutHit, TraceStart, TraceEnd, SweepRadius);

#if ENABLE_DRAW_DEBUG
			if (RFConsoleVariables::DrawBulletTraceDuration > 0.0f)
			{
				// To Sweep
				static float DebugThickness = 1.0f;
				DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Green, false, RFConsoleVariables::DrawBulletTraceDuration, 0, DebugThickness);
			}
#endif // ENABLE_DRAW_DEBUG

			//the target is a player -> add to OutHit
			if (SweepHitInfo.bBlockingHit && IsHitPlayer(SweepHitInfo))
			{
				auto Pred = [&SweepHitInfo](const FHitResult& Element) {
					return SweepHitInfo.HitObjectHandle == Element.HitObjectHandle;
				};

				if (!OutHit.ContainsByPredicate(Pred))
				{
					OutHit.Add(SweepHitInfo);
				}
			}
		}
	}
}

FHitResult URFGameplayAbility_Ranged::BulletTrace(OUT TArray<FHitResult>& OutHit, const FVector& TraceStart, const FVector& TraceEnd, float SweepRadius)
{
	TArray<FHitResult> BulletHits;
	FCollisionQueryParams TraceParams(SCENE_QUERY_STAT(BulletTrace), /*bTraceComplex=*/ true, /*IgnoreActor=*/ GetAvatarActorFromActorInfo());
	// Ignore all attached actor(e.g. weapon, equipment ...)
	TArray<AActor*> ChildAvatarActors;
	GetAvatarActorFromActorInfo()->GetAttachedActors(ChildAvatarActors);
	TraceParams.AddIgnoredActors(ChildAvatarActors);
	// Used for hit decal
	TraceParams.bReturnPhysicalMaterial = true;
	if (SweepRadius > 0.f)
	{
		GetWorld()->SweepMultiByChannel(BulletHits, TraceStart, TraceEnd, FQuat::Identity, TraceChannel_Weapon_Ranged, FCollisionShape::MakeSphere(SweepRadius));
	}
	else
	{
		GetWorld()->LineTraceMultiByChannel(BulletHits, TraceStart, TraceEnd, TraceChannel_Weapon_Ranged);
	}

	FHitResult HitInfo(EForceInit::ForceInit);

	if (BulletHits.Num() > 0)
	{
		TSet<FActorInstanceHandle> SetHits;
		for (FHitResult Hit : OutHit)
		{
			SetHits.Add(Hit.HitObjectHandle);
		}

		// Remove Same Actor on OutHit
		if (SetHits.Num() > 0)
		{
			BulletHits.RemoveAll([&SetHits](const FHitResult& Element) {
				return !SetHits.Find(Element.HitObjectHandle);
				});
			BulletHits.Shrink();
		}

		// Add HitInfo when bullet hit is valid
		if (BulletHits.Num() > 0)
		{
			OutHit.Append(BulletHits);

			// temparary HitInfo is 0 index element
			HitInfo = OutHit.GetData()[0];
		}
	}

	return HitInfo;
}

bool URFGameplayAbility_Ranged::IsHitPlayer(const FHitResult& InHitResult)
{
	return InHitResult.HitObjectHandle.DoesRepresentClass(APawn::StaticClass());
}

void URFGameplayAbility_Ranged::CalculateCameraTransform(OUT FVector& OutLocation, OUT FRotator& OutRotation)
{
	const APawn* AvatarPawn = Cast<APawn>(GetAvatarActorFromActorInfo());

	if (AvatarPawn != nullptr)
	{
		AController* Controller = AvatarPawn->Controller;

		if (APlayerController* PC = Cast<APlayerController>(Controller))
		{
			FVector CamLoc;
			FRotator CamRot;

			PC->GetPlayerViewPoint(/*out*/ CamLoc, /*out*/ CamRot);
			OutLocation = CamLoc;
			OutRotation = CamRot;
		}
		else /* AI */
		{
			OutLocation = AvatarPawn->GetActorLocation() + FVector(0, 0, AvatarPawn->BaseEyeHeight);
			OutRotation = Controller->GetControlRotation();
		}
	}
}

const FVector URFGameplayAbility_Ranged::GetCameraLocation()
{
	FVector CamLoc;
	FRotator CamRot;
	CalculateCameraTransform(CamLoc, CamRot);

	return CamLoc;
}

const FRotator URFGameplayAbility_Ranged::GetCameraRotation()
{
	FVector CamLoc;
	FRotator CamRot;
	CalculateCameraTransform(CamLoc, CamRot);

	return CamRot;
}

const FVector URFGameplayAbility_Ranged::ApplyRecoilToBulletDirection(const FVector& InDirection, float SpreadHalfAngle, float Cohesiveness)
{
	const float ActualCohesiveness = FMath::Pow(FMath::FRand(), Cohesiveness);
	const float ActualYawRecoil = ActualCohesiveness * SpreadHalfAngle;

	// The current angle is one way to the right,
	// rotating this angle from 0 to 360 to represent it in various directions.
	const float RollRecoil = FMath::FRandRange(0.f, 360.f);

	FQuat DirectionQuat(InDirection.Rotation());
	FQuat YawRecoilQuat(FRotator(0.f, ActualYawRecoil, 0.f));
	FQuat RollRecoilQuat(FRotator(0.f, 0.f, RollRecoil));
	FQuat ResultQuat = DirectionQuat * RollRecoilQuat * YawRecoilQuat;
	ResultQuat.Normalize();

	return ResultQuat.RotateVector(FVector::ForwardVector);
}
