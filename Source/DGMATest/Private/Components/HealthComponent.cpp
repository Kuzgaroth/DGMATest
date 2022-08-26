// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/HealthComponent.h"
#include "Net/UnrealNetwork.h"

UHealthComponent::UHealthComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	
	if (HealthLogDescription.IsEmpty()) HealthLogDescription = "Health is at";
	if (DeathLogDescription.IsEmpty()) DeathLogDescription = "Character has died";
	
}

void UHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	const FProperty* ReplicatedProperty = GetReplicatedProperty(StaticClass(), StaticClass(),
		GET_MEMBER_NAME_CHECKED(UHealthComponent,CurrentHealth));
	const FDoRepLifetimeParams RepParams;
	RegisterReplicatedLifetimeProperty(ReplicatedProperty, OutLifetimeProps, RepParams);
}

void UHealthComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	OnDied().Clear();
	OnHealthUpdated().Clear();
	
	Super::EndPlay(EndPlayReason);
}

void UHealthComponent::OnNewHealth(const float NewHealth)
{
	FString Role;
	if (GetOwner()->GetLocalRole()==ROLE_Authority)
	{
		Role = "Server: ";
		CurrentHealth = NewHealth;
		if (FMath::IsNearlyZero(CurrentHealth))
		{
			LogString(DeathLogDescription);
			OnDiedEvent.Broadcast(GetOwner()->GetInstigatorController());
		}
	}
	else
	{
		Role = "Client: ";
		OnHealthUpdatedEvent.Broadcast(CurrentHealth/MaxHealth);
	}
	LogFloat(CurrentHealth, Role.Append(HealthLogDescription));
}

void UHealthComponent::HandleDamageEvent(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
										 AController* InstigatedBy, AActor* DamageCauser)
{
	if (GetOwner()==DamagedActor)
	{
		if (Damage<0) return;

		Damage = FMath::Clamp(Damage, 0.f, CurrentHealth);
		const auto NewHealth = CurrentHealth-Damage;
		
		OnNewHealth(NewHealth);
	}
}

void UHealthComponent::OnRep_Health(float NewHealth)
{
	OnNewHealth(NewHealth);
}

void UHealthComponent::SetDefaults()
{
	if (GetNetMode() == NM_Client) OnHealthUpdatedEvent.Broadcast(CurrentHealth);
	else OnNewHealth(MaxHealth);
}
