// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DGMAHUD.h"
#include "UI/PlayerGameWidget.h"

void ADGMAHUD::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (PlayerWidgetClass)
	{
		PlayerWidget = CreateWidget<UPlayerGameWidget>(GetWorld(), PlayerWidgetClass,
			"PlayerWidget");
		PlayerWidget->AddToViewport();
	}
}

TDelegate<void(float)> ADGMAHUD::GetHealthDelegate()
{
	
	TDelegate<void(float)> HealthDelegate;
	const auto HealthBar = PlayerWidget->GetHealthBarWidget();
	HealthDelegate.BindUObject(HealthBar, &UHeathBarWidget::UpdateHealthBar);
	return HealthDelegate;
}

TDelegate<void(int32, int32)>& ADGMAHUD::GetWeaponDelegate() const
{
	return PlayerWidget->GetWeaponWidget()->GetAmmoDelegate();
}