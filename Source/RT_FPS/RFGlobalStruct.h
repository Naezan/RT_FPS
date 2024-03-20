// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "GameplayTagContainer.h"
#include "RFGlobalStruct.generated.h"

USTRUCT(BlueprintType)
struct RT_FPS_API FMagazineInfo
{
	GENERATED_BODY()

	FMagazineInfo() : AmmoCount(0), MagazineMesh(nullptr)
	{}

	FMagazineInfo(int32 InAmmoCount, UStaticMesh* InMesh) : AmmoCount(InAmmoCount), MagazineMesh(InMesh)
	{}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 AmmoCount;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (EditCondition = "bOverrideMagazineMesh"))
	UStaticMesh* MagazineMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Meta = (InlineEditConditionToggle))
	uint32 bOverrideMagazineMesh : 1;
};

USTRUCT(BlueprintType)
struct RT_FPS_API FEquipMagazineInfo
{
	GENERATED_BODY()

	FEquipMagazineInfo() {}

	FEquipMagazineInfo(const FEquipMagazineInfo& Other)
	{
		CurrentMagazineIndex = Other.CurrentMagazineIndex;
		EquipMagazineAmmo = Other.EquipMagazineAmmo;
	}

	FEquipMagazineInfo& operator=(const FEquipMagazineInfo& Other)
	{
		if (this != &Other)
		{
			CurrentMagazineIndex = Other.CurrentMagazineIndex;
			EquipMagazineAmmo = Other.EquipMagazineAmmo;
		}

		return *this;
	}

	int32 GetCurrentMagazineCapacity() const { return EquipMagazineAmmo.Num(); }

	int32 GetCurrentMagazineAmmo() const
	{
		return EquipMagazineAmmo.Num() > CurrentMagazineIndex ? EquipMagazineAmmo[CurrentMagazineIndex] : 0;
	}

	void RemoveCurrentMagazineAmmo(int32 StackCount)
	{
		if (EquipMagazineAmmo.Num() > 0)
		{
			if (EquipMagazineAmmo[CurrentMagazineIndex] >= StackCount)
			{
				EquipMagazineAmmo[CurrentMagazineIndex] -= StackCount;
			}
		}
	}

	void MoveNextMagazineIndex()
	{
		CurrentMagazineIndex = CurrentMagazineIndex + 1 >= EquipMagazineAmmo.Num() ? 0 : CurrentMagazineIndex + 1;
	}

	void RemoveCurrentMagazine()
	{
		if (EquipMagazineAmmo.IsValidIndex(CurrentMagazineIndex))
		{
			RemoveMagazine(CurrentMagazineIndex);
		}
	}
	void RemoveMagazine(int32 MagazineIndex)
	{
		// Block to remove last remain magazine
		if (EquipMagazineAmmo.Num() > 1)
		{
			EquipMagazineAmmo.RemoveAt(MagazineIndex);
		}

		CurrentMagazineIndex = CurrentMagazineIndex >= EquipMagazineAmmo.Num() ? EquipMagazineAmmo.Num() - 1 : CurrentMagazineIndex;
	}

	UPROPERTY(BlueprintReadOnly)
	int32 CurrentMagazineIndex;
	UPROPERTY(BlueprintReadOnly)
	TArray<int32> EquipMagazineAmmo;
};