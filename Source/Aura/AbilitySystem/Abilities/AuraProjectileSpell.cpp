// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

FString UAuraProjectileSpell::GetDescription(int32 Level) const
{
	const auto& AuraTags = FAuraGameplayTags::Get();
	const int32 Damage = DamageTypes[AuraTags.Damage_Fire].GetValueAtLevel(Level);
	if(Level == 1)
	{
		
		return FString::Printf(
			TEXT("<Title>FIRE BOLT</>\n\n<Default>Launched a bolt of fire, exploding on impact and dealing: </><Damage>%d</><Default> fire damage with a chance to burn</>\n\n<Small>Level: </><Level>%d</>"),
			Damage, Level);
	}	
	else
	{
		return FString::Printf(
			TEXT("<Title>FIRE BOLT</>\n\n<Default>Launched %d bolts of fire, exploding on impact and dealing: </><Damage>%d</><Default> fire damage with a chance to burn</>\n\n<Small>Level: </><Level>%d</>"),
			FMath::Min(NumProjectiles, Level), Damage, Level);
	}
}

FString UAuraProjectileSpell::GetNextLevelDescription(int32 Level) const
{
	++Level;
	const auto& AuraTags = FAuraGameplayTags::Get();
	const int32 Damage = DamageTypes[AuraTags.Damage_Fire].GetValueAtLevel(Level);
	return FString::Printf(
			TEXT("<Title>Next Level</>\n\n<Default>Launched %d bolts of fire, exploding on impact and dealing: </><Damage>%d</><Default> fire damage with a chance to burn</>\n\n<Small>Level: </><Level>%d</>"),
			FMath::Min(NumProjectiles, Level), Damage, Level);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag, bool bOverridePitch, float PitchOverride)
{
	if(!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		if(bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}
		
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		const auto SpawnedProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetAvatarActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//Give the Projectile a gameplay effect for causing damage
		if(const auto SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()))
		{
			check(DamageEffectClass);
			FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
			EffectContextHandle.SetAbility(this);
			EffectContextHandle.AddSourceObject(SpawnedProjectile);

			const FGameplayEffectSpecHandle EffectSpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContextHandle);
			for (const auto& [DamageTag, DamageScalableValue] : DamageTypes)
			{
				const float ScaledDamage = DamageScalableValue.GetValueAtLevel(GetAbilityLevel());
				UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(EffectSpecHandle, DamageTag, ScaledDamage);	
			}
			
			SpawnedProjectile->DamageEffectSpecHandle = EffectSpecHandle;
		}
		
		SpawnedProjectile->FinishSpawning(SpawnTransform);
	}
}
