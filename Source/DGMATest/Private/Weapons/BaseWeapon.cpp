
#include "Weapons/BaseWeapon.h"

#include "Net/UnrealNetwork.h"

ABaseWeapon::ABaseWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	RootComponent = WeaponMesh;

	bReplicates = true;
}

void ABaseWeapon::StartFire_Implementation(){}

void ABaseWeapon::StopFire_Implementation(){}

void ABaseWeapon::SetActive(bool bNewActive)
{
	if (bNewActive)
	{
		if (GetNetOwner()->GetInstigatorController() && GetNetMode() == NM_Client)
			OnAmmoChangedEvent.Broadcast(CurrentAmmo.ClipBullets, CurrentAmmo.TotalBullets);
	}
	else
	{
		OnReloadEvent.Clear();
		OnAmmoChangedEvent.Clear();
	}
}

void ABaseWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
}

void ABaseWeapon::MakeShot()
{
	ConsumeAmmo();
}

void ABaseWeapon::ConsumeAmmo()
{
	if (CurrentAmmo.ClipBullets==0)
	{
		StopFire();
		return;
	}
	--CurrentAmmo.ClipBullets;
	UpdateAmmo_Client(CurrentAmmo.ClipBullets, CurrentAmmo.TotalBullets);
	if (CurrentAmmo.ClipBullets==0 && CurrentAmmo.TotalBullets>0)
	{
		StopFire();
		Reload_All();
	}
	
}

void ABaseWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd){}

bool ABaseWeapon::CanFire() const
{
	return CurrentAmmo.ClipBullets>0;
}

bool ABaseWeapon::CanReload() const
{
	return CurrentAmmo.ClipBullets<MaxAmmo.ClipBullets && CurrentAmmo.TotalBullets>0;
}

void ABaseWeapon::ForceReload_Implementation()
{
	OnReloadEvent.Broadcast(ReloadAnim);
}

void ABaseWeapon::ReloadAmmo_Implementation()
{
	const auto AmmoToClip = MaxAmmo.ClipBullets - CurrentAmmo.ClipBullets;
	if (CurrentAmmo.TotalBullets>=AmmoToClip)
	{
		CurrentAmmo.ClipBullets = MaxAmmo.ClipBullets;
		CurrentAmmo.TotalBullets -= AmmoToClip;
	}
	else
	{
		CurrentAmmo.ClipBullets = CurrentAmmo.ClipBullets+CurrentAmmo.TotalBullets;
		CurrentAmmo.TotalBullets = 0;
	}

	UpdateAmmo_Client(CurrentAmmo.ClipBullets, CurrentAmmo.TotalBullets);
}

void ABaseWeapon::UpdateAmmo_Client_Implementation(int32 CurrentClip, int32 CurrentTotal)
{
	CurrentAmmo.ClipBullets = CurrentClip;
	CurrentAmmo.TotalBullets = CurrentTotal;
	OnAmmoChangedEvent.Broadcast(CurrentClip, CurrentTotal);
}

void ABaseWeapon::SetAmmo(FAmmoData NewAmmoData)
{
	CurrentAmmo = NewAmmoData;
}

void ABaseWeapon::GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const
{
	const auto Controller = GetInstigatorController();
	
	if (Controller->IsPlayerController())
	{
		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}
	else
	{
		ViewLocation = GetMuzzleWorldLocation();
		ViewRotation = WeaponMesh->GetSocketRotation(MuzzelSocketName);
	}
}

FVector ABaseWeapon::GetMuzzleWorldLocation() const
{
	return WeaponMesh->GetSocketLocation(MuzzelSocketName);
}

void ABaseWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	SetActive(false);
	Super::EndPlay(EndPlayReason);
}

void ABaseWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	CurrentAmmo.ClipBullets = MaxAmmo.ClipBullets;
	CurrentAmmo.TotalBullets = MaxAmmo.TotalBullets-MaxAmmo.ClipBullets;
}

void ABaseWeapon::Reload_All_Implementation()
{
	OnReloadEvent.Broadcast(ReloadAnim);
}
