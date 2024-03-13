// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/RFGameplayAbility_Ranged.h"
#include "RFWeaponInstance.h"
#include "RFLogMacros.h"
#include "Collision\RFCollisionChannel.h"

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

	//SendTargetDataToServer();
}

URFWeaponInstance* URFGameplayAbility_Ranged::GetWeaponInstance() const
{
	return Cast<URFWeaponInstance>(GetSourceObject(CurrentSpecHandle, CurrentActorInfo));
}

void URFGameplayAbility_Ranged::PerformBulletTrace(OUT TArray<FHitResult>& OutHit)
{
	URFWeaponInstance* WeaponData = GetWeaponInstance();

	FVector TraceStart = WeaponData->GetMuzzleLocation();
	FVector CameraDir = GetCameraRotation().RotateVector(FVector::ForwardVector);
	FVector TraceEnd = TraceStart + CameraDir * WeaponData->GetFireEffectiveRange();

	int32 CartridgeCount = WeaponData->GetOneShotCartridge();
	float SweepRadius = WeaponData->GetFireTraceRadius();
	BulletTraceByCartridge(OutHit, TraceStart, TraceEnd, CartridgeCount, SweepRadius);
}

void URFGameplayAbility_Ranged::BulletTraceByCartridge(OUT TArray<FHitResult>& OutHit, const FVector& TraceStart, const FVector& TraceEnd, int32 InCartridgeCount, float SweepRadius)
{
	for (int i = 0; i < InCartridgeCount; ++i)
	{
		// Do first SingleLineTrace for simple to found target
		FHitResult HitInfo = BulletTrace(OutHit, TraceStart, TraceEnd, 0.f);

		if (!HitInfo.bBlockingHit)
		{
			// Perform SweepLineTrace if none of the Hits have been detected
			FHitResult SweepHitInfo = BulletTrace(OutHit, TraceStart, TraceEnd, SweepRadius);

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
