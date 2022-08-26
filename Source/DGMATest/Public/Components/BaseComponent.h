// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BaseComponent.generated.h"

DECLARE_LOG_CATEGORY_CLASS(LogComponent, All, All)

UCLASS( ClassGroup=(Custom), meta=(ChildCannotTick), Abstract )
class DGMATEST_API UBaseComponent : public UActorComponent
{
	GENERATED_BODY()
public:	
	UBaseComponent(const FObjectInitializer& ObjectInitializer);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
protected:
	void LogFloat(const float Value, const FString& Description);
	void LogString(const FString& Description);
};
