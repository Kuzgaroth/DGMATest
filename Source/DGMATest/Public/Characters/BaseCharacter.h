// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DGMATest/Public/CoreTypes.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

class UHealthComponent;
class UWeaponComponent;
class UWidgetComponent;

UCLASS()
class DGMATEST_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	UWidgetComponent* HealthWidget;

	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
public:
	virtual void ConnectHealthComponent_Server(const TDelegate<void(AController*)>& DeathDelegate);
	virtual void ConnectHealthComponent_Client(const TDelegate<void(float)>& HealthDelegate);
	
	virtual void ConnectWeaponComponent_Client(TDelegate<void(int32, int32)>& AmmoDelegate);
	virtual void Restart() override;
};

