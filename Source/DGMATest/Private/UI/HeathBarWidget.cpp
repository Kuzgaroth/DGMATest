// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HeathBarWidget.h"
#include "Components/ProgressBar.h"

void UHeathBarWidget::UpdateHealthBar(float NewValue)
{
	HealthBar->SetPercent(NewValue);
}
