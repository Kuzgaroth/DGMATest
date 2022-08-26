#include "DGMATestGameMode.h"

ADGMATestGameMode::ADGMATestGameMode(){}

void ADGMATestGameMode::StartRespawn(AController* NewController, FTimerHandle RespawnHandle)
{
	RestartPlayer(NewController);
	RespawnMap.Remove(NewController);
	GetWorldTimerManager().ClearTimer(RespawnHandle);
}

void ADGMATestGameMode::RequestRespawn(AController* PC)
{
	PC->GetPawn()->Reset();
	PC->ChangeState(NAME_Spectating);
	FTimerHandle RespawnHandle;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ADGMATestGameMode::StartRespawn, PC, RespawnHandle);
	GetWorldTimerManager().SetTimer(RespawnHandle, TimerDelegate, RespawnTime, false);

	RespawnMap.Add(PC, MoveTemp(RespawnHandle));
}
