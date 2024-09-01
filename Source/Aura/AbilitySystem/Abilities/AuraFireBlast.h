// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "AuraFireBlast.generated.h"

class AAuraFireBall;
/**
 * 
 */
UCLASS()
class AURA_API UAuraFireBlast : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Fire Blast")
	int32 NumFireBalls = 12;

	UPROPERTY(EditDefaultsOnly, Category = "Fire Blast")
	TSubclassOf<AAuraFireBall> FireBallClass;

public:

	virtual FString GetDescription(int32 CurrentLevel) const override;
	virtual FString GetNextLevelDescription(int32 CurrentLevel) const override;

	UFUNCTION(BlueprintCallable)
	TArray<AAuraFireBall*> SpawnFireBalls();
};
