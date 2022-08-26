// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeathBarWidget.h"
#include "Blueprint/UserWidget.h"
#include "UI/WeaponWidget.h"
#include "PlayerGameWidget.generated.h"

class UCanvasPanel;

UCLASS()
class DGMATEST_API UPlayerGameWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UWeaponWidget> WeaponWidgetClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UHeathBarWidget> HealthWidgetClass;

	UPROPERTY(meta=(BindWidget))
	UCanvasPanel* Canvas;
	virtual void NativeOnInitialized() override;
private:
	UPROPERTY()
	UWeaponWidget* WeaponWidget;
	UPROPERTY()
	UHeathBarWidget* HealthBarWidget;
public:
	FORCEINLINE UHeathBarWidget* GetHealthBarWidget() const {return HealthBarWidget;}
	FORCEINLINE UWeaponWidget* GetWeaponWidget() const {return WeaponWidget;}
};