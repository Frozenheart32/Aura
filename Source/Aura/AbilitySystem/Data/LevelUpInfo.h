// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"


USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 LevelUpRequirement = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 AttributePointAward = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SpellPointsAward = 1;
};

/**
 * 
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FAuraLevelUpInfo> LevelUpInformation;

	UFUNCTION(BlueprintCallable)
	int32 FindLevelForXP(int32 XP) const;
};
