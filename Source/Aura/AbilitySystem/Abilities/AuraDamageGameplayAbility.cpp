// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraDamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilityTypes.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"

FTaggedMontage UAuraDamageGameplayAbility::GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const
{
	if(TaggedMontages.Num() > 0)
	{
		const int32 Selection = FMath::RandRange(0, TaggedMontages.Num() - 1);
		return TaggedMontages[Selection];
	}

	return FTaggedMontage{};
}

void UAuraDamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	const auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(!TargetASC) return;

	const FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass, 1.f);

	check(DamageType.IsValid());

	const FDamageEffectParams Params = MakeDamageEffectParamsFromClassDefaults(TargetActor);
	UAuraAbilitySystemLibrary::ApplyDamageEffect(Params);
}

FDamageEffectParams UAuraDamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(
		AActor* TargetActor, FVector InRadialDamageOrigin,
		bool bOverrideKnockbackDirection,FVector InKnockbackDirectionOverride,
		bool bOverrideDeathImpulse, FVector InDeathImpulseOverride,
		bool bOverridePitch, float InPitchOverride) const
{
	FDamageEffectParams Params{};
	Params.WorldContextObject = GetAvatarActorFromActorInfo();
	Params.DamageGameplayEffectClass = DamageEffectClass;
	Params.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	Params.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	Params.BaseDamage = GetDamageAtLevel();
	Params.AbilityLevel = GetAbilityLevel();
	Params.DamageType = DamageType;
	Params.DebuffChance = DebuffChance;
	Params.DebuffDamage = DebuffDamage;
	Params.DebuffDuration = DebuffDuration;
	Params.DebuffFrequency = DebufFrequency;
	Params.DeathImpulseMagnitude = DeathImpulseMagnitude;
	Params.KnockbackForceMagnitude = KnockbackForceMagnitude;
	Params.KnockbackChance = KnockbackChance;

	if(bIsRadialDamage)
	{
		Params.bIsRadialDamage = bIsRadialDamage;
		Params.RadialDamageOrigin = InRadialDamageOrigin;
		Params.RadialDamageInnerRadius = RadialDamageInnerRadius;
		Params.RadialDamageOuterRadius = RadialDamageOuterRadius;
	}
	
	if(bOverrideKnockbackDirection)
	{
		InKnockbackDirectionOverride.Normalize();
		Params.KnockbackForce = InKnockbackDirectionOverride + KnockbackForceMagnitude;
		if(bOverridePitch)
		{
			FRotator KnockbackRotation = InKnockbackDirectionOverride.Rotation();
			KnockbackRotation.Pitch = InPitchOverride;
			Params.KnockbackForce = KnockbackRotation.Vector() * KnockbackForceMagnitude;
		}
	}
	else
	{
		if(IsValid(TargetActor))
		{
			FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
			const FVector ToTarget = Rotation.Vector();
			Rotation.Pitch = 45.f;
			if(bOverridePitch)
			{
				Rotation.Pitch = InPitchOverride;
			}
			Params.KnockbackForce = ToTarget * KnockbackForceMagnitude;
		}
	}

	if(bOverrideDeathImpulse)
	{
		InDeathImpulseOverride.Normalize();
		Params.DeathImpulse = InDeathImpulseOverride + DeathImpulseMagnitude;
		if(bOverridePitch)
		{
			FRotator DeathImpulseRotation = InDeathImpulseOverride.Rotation();
			DeathImpulseRotation.Pitch = InPitchOverride;
			Params.DeathImpulse = DeathImpulseRotation.Vector() * DeathImpulseMagnitude;
		}
	}
	else
	{
		if(IsValid(TargetActor))
		{
			FRotator Rotation = (TargetActor->GetActorLocation() - GetAvatarActorFromActorInfo()->GetActorLocation()).Rotation();
			const FVector ToTarget = Rotation.Vector();
			Rotation.Pitch = 45.f;
			if(bOverridePitch)
			{
				Rotation.Pitch = InPitchOverride;
			}
			Params.DeathImpulse = ToTarget * DeathImpulseMagnitude;
		}
	}
	

	return Params;
}

float UAuraDamageGameplayAbility::GetDamageAtLevel() const
{
	return Damage.GetValueAtLevel(GetAbilityLevel());
}
