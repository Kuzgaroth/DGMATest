// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class UProjectileMovementComponent;

UCLASS(meta=(ChildCannotTick))
class DGMATEST_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
public:
	ABaseProjectile();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Components")
	UStaticMeshComponent* ProjectileMesh;
	
	UPROPERTY(VisibleDefaultsOnly, Category="Components")
	UProjectileMovementComponent* MovementComponent;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,Category="Damage")
	float DamageRadius = 200.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,Category="Damage")
	float DamageAmount = 100.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite ,Category="Damage")
	bool bDoFullDamage;
	FVector ShotDirection;
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
	virtual void PostInitializeComponents() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	void SetShotDirection(const FVector& Direction){ShotDirection = Direction;};

};
