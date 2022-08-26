// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HeathBarWidget.generated.h"

class UProgressBar;

UCLASS()
class DGMATEST_API UHeathBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta=(BindWidget))
	UProgressBar* HealthBar;
public:
	void UpdateHealthBar(float NewValue);
};
