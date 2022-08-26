// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DGMAController.h"
#include "DGMATestGameMode.h"
#include "Characters/BaseCharacter.h"
#include "UI/DGMAHUD.h"

void ADGMAController::OnPossess(APawn* InPawn)
{
	
	if (InPawn)
	{
		const auto Char = Cast<ABaseCharacter>(InPawn);
		if (Char)
		{
			const auto GameMode = GetWorld()->GetAuthGameMode<ADGMATestGameMode>();
			if (GameMode)
			{
				const auto DeathDelegate = TDelegate<void(AController*)>::CreateUObject(GameMode,
					&ADGMATestGameMode::RequestRespawn);
				Char->ConnectHealthComponent_Server(DeathDelegate);
			
			}
		}
	}
	Super::OnPossess(InPawn);
}

void ADGMAController::OnUnPossess()
{
	Super::OnUnPossess();
	if (GetPawn()!=nullptr) GetPawn()->Destroy();
}

void ADGMAController::ClientRestart_Implementation(APawn* NewPawn)
{
	
	const auto Char = Cast<ABaseCharacter>(NewPawn);
	const auto HUDObj = GetHUD<ADGMAHUD>();

	if (Char && HUDObj)
	{
		Char->ConnectHealthComponent_Client(HUDObj->GetHealthDelegate());
		Char->ConnectWeaponComponent_Client(HUDObj->GetWeaponDelegate());
	}

	Super::ClientRestart_Implementation(NewPawn);
}
