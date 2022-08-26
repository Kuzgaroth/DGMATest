// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Projectiles/BaseProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileMesh");
	RootComponent = ProjectileMesh;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComponent");

	bReplicates =true;
}

void ABaseProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	MovementComponent->StopMovementImmediately();
	
	UGameplayStatics::ApplyRadialDamage(GetWorld(), DamageAmount, GetActorLocation() , DamageRadius,
		UDamageType::StaticClass(), {GetOwner()}, this, GetInstigator()->GetController(),
		bDoFullDamage);
	
	Destroy();
}

void ABaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	ProjectileMesh->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnProjectileHit);
	MovementComponent->Velocity=ShotDirection*MovementComponent->InitialSpeed;
	
}

void ABaseProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	ProjectileMesh->OnComponentHit.Clear();
	Super::EndPlay(EndPlayReason);
}
