// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraDamageGameplayAbility.generated.h"

struct FDamageEffectParams;
struct FTaggedMontage;
/**
 * 
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	FScalableFloat Damage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DeathImpulseMagnitude = 3000.f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Radial Damage")
	bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Radial Damage")
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage|Radial Damage")
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "Knockback")
	float KnockbackForceMagnitude = 1000.f;
	UPROPERTY(EditDefaultsOnly, Category = "Knockback")
	float KnockbackChance = 0.f;


	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	float DebuffChance = 20.f;

	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	float DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	float DebufFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, Category = "Debuff")
	float DebuffDuration;


	UFUNCTION(BlueprintCallable, meta=(BlueprintProtected))
	FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

public:
	
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintCallable)
	FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(
		AActor* TargetActor = nullptr,
		FVector InRadialDamageOrigin = FVector::ZeroVector,
		bool bOverrideKnockbackDirection = false,
		FVector InKnockbackDirectionOverride = FVector::ZeroVector,
		bool bOverrideDeathImpulse = false,
		FVector InDeathImpulseOverride = FVector::ZeroVector,
		bool bOverridePitch = false,
		float InPitchOverride = 45.f) const;

	UFUNCTION(BlueprintCallable)
	float GetDamageAtLevel() const;
};
