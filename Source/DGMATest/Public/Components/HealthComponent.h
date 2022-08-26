// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseComponent.h"
#include "HealthComponent.generated.h"

DECLARE_EVENT_OneParam(UHealthComponent, FOnDiedEvent, AController*)
DECLARE_EVENT_OneParam(UHealthComponent, FOnHealthUpdatedEvent, float)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DGMATEST_API UHealthComponent : public UBaseComponent
{
	GENERATED_BODY()

public:	
	UHealthComponent(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(ClampMin=0.f))
	float MaxHealth = 300.f;
	UPROPERTY(Replicated, ReplicatedUsing=OnRep_Health)
	float CurrentHealth = 0.f;

	UPROPERTY(EditDefaultsOnly, Category="Log")
	FString HealthLogDescription;

	UPROPERTY(EditDefaultsOnly, Category="Log")
	FString DeathLogDescription;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
private:
	FOnDiedEvent OnDiedEvent;
	FOnHealthUpdatedEvent OnHealthUpdatedEvent;
	
	void OnNewHealth(const float NewHealth);
public:
	UFUNCTION()
	void HandleDamageEvent(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
		AActor* DamageCauser);
	UFUNCTION()
	void OnRep_Health(float NewHealth);
	void SetDefaults();
	FOnDiedEvent& OnDied() {return OnDiedEvent;}
	FOnHealthUpdatedEvent& OnHealthUpdated() {return OnHealthUpdatedEvent;}
};
