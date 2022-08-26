// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/WeaponComponent.h"
#include "GameFramework/Character.h"
#include "Weapons/BaseWeapon.h"


UWeaponComponent::UWeaponComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;

	bWantsInitializeComponent = true;
}

void UWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (const auto Weapon : Weapons)  Weapon->Destroy();
	CurrentWeapon = nullptr;
	Super::EndPlay(EndPlayReason);
}

void UWeaponComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	for (const auto WeaponClass : WeaponClasses)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.Instigator = GetOwner<APawn>();
		SpawnParameters.Owner = GetOwner();
		auto Weapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, SpawnParameters);
		if (Weapon)
		{
			Weapons.Add(Weapon);
			Weapon->SetActorEnableCollision(false);
			Weapon->SetHidden(true);
			Weapon->AttachToComponent(GetOwner<ACharacter>()->GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale, ArmorySocketName);
		}
	}
	if (Weapons.Num()>0) CurrentWeapon = Weapons[0];	
}

void UWeaponComponent::SetCurrentWeapon(ABaseWeapon* NewWeapon)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->SetActive(false);
		CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
		CurrentWeapon->AttachToComponent(GetOwner<ACharacter>()->GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale, ArmorySocketName);
	}
	
	CurrentWeapon = NewWeapon;
	CurrentWeapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	if (GetOwnerRole()==ROLE_Authority)
		CurrentWeapon->OnReload().AddUObject(this, &UWeaponComponent::ReloadWeapon);
	if (GetNetMode()==NM_Client && GetOwner()->GetInstigatorController()) CurrentWeapon->OnAmmoChanged().Add(AmmoDelegate);
	CurrentWeapon->AttachToComponent(GetOwner<ACharacter>()->GetMesh(),
			FAttachmentTransformRules::SnapToTargetIncludingScale, MainHandSocketName);
	CurrentWeapon->SetActive(true);
	CurrentWeapon->SetHidden(false);
}

void UWeaponComponent::ReloadWeapon_Implementation(UAnimMontage* ReloadAnim)
{
	if (GetOwnerRole()==ROLE_Authority) return;
	if (ReloadAnim)
	{
		const auto AnimInst =GetOwner<ACharacter>()->GetMesh()->GetAnimInstance();
		AnimInst->Montage_Play(ReloadAnim);
		FOnMontageBlendingOutStarted MontageDelegate = FOnMontageBlendingOutStarted::CreateLambda(
			[&](UAnimMontage* Anim, bool bInterrupted)
		{
			if (GetOwner()->GetInstigatorController() && GetOwnerRole()<ROLE_Authority)
			{
				CurrentWeapon->ReloadAmmo();
			}
			bReloading = false;
		});
		AnimInst->Montage_SetBlendingOutDelegate(MontageDelegate, ReloadAnim);
		bReloading = true;
	} else if (GetOwner()->GetInstigatorController()) CurrentWeapon->ReloadAmmo();
	
}

void UWeaponComponent::SetAmmoDelegate(TDelegate<void(int32, int32)>& Delegate)
{
	AmmoDelegate = MoveTemp(Delegate);
}

void UWeaponComponent::SetCurrentWeapon()
{
	if (CurrentWeapon) SetCurrentWeapon(CurrentWeapon);
}

void UWeaponComponent::StartFire()
{
	if (CurrentWeapon && CurrentWeapon->CanFire() && !bReloading) CurrentWeapon->StartFire();
	
}

void UWeaponComponent::StopFire()
{
	if (CurrentWeapon) CurrentWeapon->StopFire();
}

void UWeaponComponent::Reload()
{
	if (CurrentWeapon && CurrentWeapon->CanReload() && !bReloading) CurrentWeapon->ForceReload();
}

void UWeaponComponent::NextWeapon()
{
	if (Weapons.Num()<2) return;
	const auto NewWeapon = Weapons[(Weapons.IndexOfByKey(CurrentWeapon)+1) % Weapons.Num()];
	SetCurrentWeapon(NewWeapon);
}
