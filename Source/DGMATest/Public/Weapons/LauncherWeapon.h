// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/BaseWeapon.h"
#include "LauncherWeapon.generated.h"

class ABaseProjectile;

UCLASS()
class DGMATEST_API ALauncherWeapon : public ABaseWeapon
{
	GENERATED_BODY()

public:
	ALauncherWeapon(const FObjectInitializer& ObjectInitializer);
protected:
	UPROPERTY(EditDefaultsOnly, Category="Projectile")
	TSubclassOf<ABaseProjectile> RocketClass;
	
	virtual void GetTraceData(FVector& TraceStart, FVector& TraceEnd) override;
public:
	virtual void StartFire() override;
	virtual void MakeShot() override;
	virtual void SetActive(bool bNewActive) override;
};
