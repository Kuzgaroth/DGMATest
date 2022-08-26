// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BaseComponent.h"
#include "DGMATest/Public/CoreTypes.h"
#include "WeaponComponent.generated.h"

class ABaseWeapon;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DGMATEST_API UWeaponComponent : public UBaseComponent
{
	GENERATED_BODY()
public:
	UWeaponComponent(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(EditDefaultsOnly, Category="Weapons")
	TArray<TSubclassOf<ABaseWeapon>> WeaponClasses;
	UPROPERTY(EditDefaultsOnly, Category="Attachement")
	FName MainHandSocketName;
	UPROPERTY(EditDefaultsOnly, Category="Attachement")
	FName ArmorySocketName;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void InitializeComponent() override;
private:
	UPROPERTY()
	ABaseWeapon* CurrentWeapon;
	UPROPERTY()
	TArray<ABaseWeapon*> Weapons;
	bool bReloading=false;
	TDelegate<void(int32, int32)> AmmoDelegate;
	
	void SetCurrentWeapon(ABaseWeapon* NewWeapon);
	UFUNCTION(NetMulticast, Reliable)
	void ReloadWeapon(UAnimMontage* ReloadMontage);
public:
	void SetAmmoDelegate(TDelegate<void(int32, int32)>& Delegate);
	void SetCurrentWeapon();
	void StartFire();
	void StopFire();
	void Reload();
	void NextWeapon();
};
