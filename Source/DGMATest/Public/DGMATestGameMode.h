#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DGMATestGameMode.generated.h"

UCLASS(minimalapi)
class ADGMATestGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ADGMATestGameMode();
protected:
	UPROPERTY(EditDefaultsOnly, Category="Respawn")
	float RespawnTime = 2.f;

private:
	void StartRespawn(AController* NewController, FTimerHandle RespawnHandle);

	UPROPERTY()
	TMap<AController*, FTimerHandle> RespawnMap;
public:
	void RequestRespawn(AController* PC);
};



