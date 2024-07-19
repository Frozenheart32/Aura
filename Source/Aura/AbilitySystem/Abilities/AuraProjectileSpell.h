// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AuraProjectileSpell.generated.h"

struct FGameplayTag;
class AAuraProjectile;
class UGameplayEffect;

/**
 * 
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumProjectiles = 5;
	
	UFUNCTION(BlueprintCallable, Category = "Projectile", meta=(BlueprintProtected))
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch = false, float PitchOverride = 0.f);
};
