// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "WeaponWidget.generated.h"

UCLASS()
class DGMATEST_API UWeaponWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* ClipAmmoForm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	UTextBlock* TotalAmmoForm;
public:
	TDelegate<void(int32, int32)>& GetAmmoDelegate();
private:
	void SetAmmoData(int32 ClipAmmo, int32 TotalAmmo) const;
	TDelegate<void(int32, int32)> AmmoDelegate;
};

