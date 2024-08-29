// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"
#include "ArcaneShards.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UArcaneShards : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Arcane Shards Settings")
	int32 MaxNumShards = 10; 


public:

	virtual FString GetDescription(int32 CurrentLevel) const override;
	virtual FString GetNextLevelDescription(int32 CurrentLevel) const override;

	UFUNCTION(BlueprintCallable)
	int32 GetMaxShardsCountAtLevel() const;
};
