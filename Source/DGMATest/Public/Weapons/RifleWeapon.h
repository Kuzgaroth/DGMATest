// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "RifleWeapon.generated.h"



UCLASS()
class DGMATEST_API ARifleWeapon : public ABaseWeapon
{
	GENERATED_BODY()

public:
	ARifleWeapon(const FObjectInitializer& ObjectInitializer);
protected:
	UPROPERTY(EditDefaultsOnly, Category="Shooting Params")
	float BulletSpread;

	UPROPERTY(EditDefaultsOnly, Category="Damage")
	float ShotDamage;
	
	virtual void GetTraceData(FVector& TraceStart, FVector& TraceEnd) override;
public:
	virtual void StartFire() override;
	virtual void StopFire() override;
	virtual void MakeShot() override;
};
