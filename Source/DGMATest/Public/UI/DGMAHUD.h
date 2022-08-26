// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "DGMAHUD.generated.h"

class UPlayerGameWidget;

UCLASS()
class DGMATEST_API ADGMAHUD : public AHUD
{
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, Category="Widgets")
	TSubclassOf<UPlayerGameWidget> PlayerWidgetClass;
	
	virtual void PostInitializeComponents() override;
public:
	TDelegate<void(float)> GetHealthDelegate();
	TDelegate<void(int32, int32)>& GetWeaponDelegate() const;
private:
	UPROPERTY()
	UPlayerGameWidget* PlayerWidget;
};
