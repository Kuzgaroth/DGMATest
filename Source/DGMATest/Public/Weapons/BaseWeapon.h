#pragma once

#include "CoreMinimal.h"
#include "DGMATest/Public/CoreTypes.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"

DECLARE_EVENT_OneParam(ABaseWeapon, FOnReloadEvent, UAnimMontage*)
DECLARE_EVENT_TwoParams(ABaseWeapon, FOnAmmoChangedEvent, int32, int32)

UCLASS()
class DGMATEST_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ABaseWeapon(const FObjectInitializer& ObjectInitializer);
protected:
	UPROPERTY(EditDefaultsOnly, Category="Components")
	USkeletalMeshComponent* WeaponMesh;
	
	UPROPERTY(EditDefaultsOnly, Category="Ammo")
	FAmmoData MaxAmmo;
	UPROPERTY(EditDefaultsOnly, Category="Animation")
	UAnimMontage* ReloadAnim;
	
	UPROPERTY(EditDefaultsOnly, Category="Shooting Params")
	float ShootingRange;
	UPROPERTY(EditDefaultsOnly, Category="Shooting Params")
	float TimeBetweenShots;
	UPROPERTY(EditDefaultsOnly, Category="Shooting Params")
	FName MuzzelSocketName;
	
	virtual void MakeShot();
	void ConsumeAmmo();
	virtual void GetTraceData(FVector& TraceStart, FVector& TraceEnd);
	void GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	FVector GetMuzzleWorldLocation() const;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void PostInitializeComponents() override;
	FTimerHandle FireHandle;

	UFUNCTION(NetMulticast, Reliable)
	void Reload_All();
	UFUNCTION(Client, Reliable)
	void UpdateAmmo_Client(int32 CurrentClip, int32 CurrentTotal);
public:
	UFUNCTION(Server, Reliable)
	void ReloadAmmo();
	void SetAmmo(FAmmoData NewAmmoData = {});
	bool CanFire() const;
	bool CanReload() const;
	UFUNCTION(Server, Reliable)
	void ForceReload();
	UFUNCTION(Server, Reliable)
	virtual void StartFire();
	UFUNCTION(Server, Reliable)
	virtual void StopFire();
	virtual void SetActive(bool bNewActive);
	FOnReloadEvent& OnReload() {return OnReloadEvent;}
	FOnAmmoChangedEvent& OnAmmoChanged() {return OnAmmoChangedEvent;}
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
private:
	UPROPERTY()
	FAmmoData CurrentAmmo;
	FOnReloadEvent OnReloadEvent;
	FOnAmmoChangedEvent OnAmmoChangedEvent;
};
