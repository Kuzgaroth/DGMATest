// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/BaseComponent.h"

UBaseComponent::UBaseComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = false;
	
}

void UBaseComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void UBaseComponent::LogFloat(const float Value, const FString& Description)
{
	UE_LOG(LogComponent, Warning, TEXT("%s: %f"), *Description, Value);
}

void UBaseComponent::LogString(const FString& Description)
{
	UE_LOG(LogComponent, Warning, TEXT("%s"), *Description);
}

