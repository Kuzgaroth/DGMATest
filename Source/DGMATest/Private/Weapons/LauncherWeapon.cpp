// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/LauncherWeapon.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Weapons/Projectiles/BaseProjectile.h"

ALauncherWeapon::ALauncherWeapon(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
}

void ALauncherWeapon::GetTraceData(FVector& TraceStart, FVector& TraceEnd)
{
	Super::GetTraceData(TraceStart, TraceEnd);

	FVector ViewLocation;
	FRotator ViewRotation;
	GetPlayerViewPoint(ViewLocation, ViewRotation);
	TraceStart = ViewLocation;
	TraceEnd = TraceStart + ViewRotation.Vector() * ShootingRange;
}

void ALauncherWeapon::StartFire()
{
	Super::StartFire();

	if (GetWorldTimerManager().IsTimerActive(FireHandle)) return;
	GetWorldTimerManager().SetTimer(FireHandle, this, &ALauncherWeapon::StopFire, TimeBetweenShots,
		false);
	MakeShot();
}

void ALauncherWeapon::MakeShot()
{
	FVector TraceStart;
	FVector TraceEnd;
	GetTraceData(TraceStart, TraceEnd);
	FHitResult HitResult;
	
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), TraceStart, TraceEnd,
		UEngineTypes::ConvertToTraceType(ECC_Visibility), false,
		{GetOwner()}, EDrawDebugTrace::ForDuration, HitResult, true);
	
	FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraceEnd;
	const FVector Direction = (EndPoint-GetMuzzleWorldLocation()).GetSafeNormal();
	const FTransform SpawnTransform(FRotator::ZeroRotator,
		WeaponMesh->GetSocketTransform(MuzzelSocketName).GetLocation());
	auto Projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(RocketClass, SpawnTransform, GetOwner(), GetOwner<APawn>());
	
	if (Projectile)
	{
		Projectile->SetShotDirection(Direction);
		Projectile->SetOwner(GetOwner());
		Projectile->FinishSpawning(SpawnTransform);
	}
	
	Super::MakeShot();
}

void ALauncherWeapon::SetActive(bool bNewActive)
{
	Super::SetActive(bNewActive);

	if (GetWorldTimerManager().IsTimerActive(FireHandle)) GetWorldTimerManager().ClearTimer(FireHandle);
}
