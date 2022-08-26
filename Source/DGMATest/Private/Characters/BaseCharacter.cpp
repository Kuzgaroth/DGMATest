// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Components/HealthComponent.h"
#include "Components/WeaponComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/HeathBarWidget.h"

ABaseCharacter::ABaseCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
	WeaponComponent->SetIsReplicated(true);
	HealthComponent->SetIsReplicated(true);

	HealthWidget = CreateDefaultSubobject<UWidgetComponent>("HealthWidget");
	HealthWidget->SetupAttachment(RootComponent);
}

void ABaseCharacter::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	if (GetNetMode()==NM_DedicatedServer) HealthWidget->SetWidgetClass(nullptr);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!GetController())
	{
		const auto HWidget = Cast<UHeathBarWidget>(HealthWidget->GetWidget());
		if (HWidget)
		{
			HealthComponent->OnHealthUpdated().AddUObject(HWidget, &UHeathBarWidget::UpdateHealthBar);
			HealthComponent->SetDefaults();
		}
	}
	else HealthWidget->SetVisibility(false);
}

void ABaseCharacter::ConnectHealthComponent_Server(const TDelegate<void(AController*)>& DeathDelegate)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		HealthComponent->OnDied().Add(DeathDelegate);
		OnTakeAnyDamage.AddDynamic(HealthComponent, &UHealthComponent::HandleDamageEvent);
	}
}

void ABaseCharacter::ConnectHealthComponent_Client(const TDelegate<void(float)>& HealthDelegate)
{
	if (!HealthComponent->OnHealthUpdated().IsBoundToObject(HealthDelegate.GetUObject()))
		HealthComponent->OnHealthUpdated().Add(HealthDelegate);
	
}

void ABaseCharacter::ConnectWeaponComponent_Client(TDelegate<void(int32, int32)>& AmmoDelegate)
{
	WeaponComponent->SetAmmoDelegate(AmmoDelegate);
}

void ABaseCharacter::Restart()
{
	Super::Restart();
	
	HealthComponent->SetDefaults();
	WeaponComponent->SetCurrentWeapon();
}
