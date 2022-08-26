
#pragma once

#include "CoreMinimal.h"
#include "CoreTypes.generated.h"

class ABaseWeapon;

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category="Ammo")
	int32 TotalBullets;

	UPROPERTY(EditDefaultsOnly, Category="Ammo")
	int32 ClipBullets;

	FAmmoData()
	{
		TotalBullets = 120;
		ClipBullets = 30;
	}
	
};