// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*Stat Value*/)

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:

	AAuraPlayerState();

	FOnPlayerStatChanged OnXPChanged;
	FOnPlayerStatChanged OnLevelChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<ULevelUpInfo> LevelUpInfo;

protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ASC")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = "OnRep_Level")
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = "OnRep_XP")
	int32 XP = 0;

public:
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const;
	
	FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	FORCEINLINE int32 GetXP() const { return XP; }

	void AddToXP(int32 InXP);
	void SetXP(int32 InXP);

	void AddLevel(int32 InLevel);
	void SetLevel(int32 InLevel);

private:

	UFUNCTION()
	void OnRep_Level(int32 OldLevel) const;
	UFUNCTION()
	void OnRep_XP(int32 OldXP) const;
};
