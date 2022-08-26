// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerGameWidget.h"

#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UPlayerGameWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (WeaponWidgetClass) WeaponWidget = CreateWidget<UWeaponWidget>(GetOwningPlayer(),
		WeaponWidgetClass, "WeaponWidget");

	if (HealthWidgetClass) HealthBarWidget = CreateWidget<UHeathBarWidget>(GetOwningPlayer(),
		HealthWidgetClass, "HealthWidget");

	if (WeaponWidget) WeaponWidget->AddToViewport();
	if (HealthBarWidget)
	{
		const auto HealthSlot = Canvas->AddChildToCanvas(HealthBarWidget);
		FVector2D ViewportSize;
		GetWorld()->GetGameViewport()->GetViewportSize(ViewportSize);
		HealthSlot->SetAnchors(FAnchors(0.f, 1.f, 0.f, 1.f));
		HealthSlot->SetPosition(FVector2D(30.f, -75.f));
		HealthSlot->SetAlignment(FVector2D(0.f, 0.f));
		HealthSlot->SetSize(FVector2D(300.f, 30.f));
	}
	
}

