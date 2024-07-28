// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/KismetSystemLibrary.h"

FString UAuraFireBolt::GetDescription(int32 CurrentLevel) const
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(CurrentLevel);
	const float ManaCost = GetManaCost(CurrentLevel);
	const float Cooldown = GetCooldown(CurrentLevel);
	if(CurrentLevel == 1)
	{
		
		return FString::Printf(
			TEXT("<Title>FIRE BOLT</>\n\n"
			
					"<Small>Level: </><Level>%d</>\n"
					
					"<Small>ManaCost: </><ManaCost>%.1f</>\n"

					"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
					
					"<Default>Launched a bolt of fire, "
					"exploding on impact and dealing: </>"
					"<Damage>%d</><Default> fire damage with a chance to burn</>\n\n"),
			CurrentLevel,
			ManaCost,
			Cooldown,
			ScaledDamage);
	}	
	else
	{
		return FString::Printf(
			TEXT("<Title>FIRE BOLT</>\n\n"
			
					"<Small>Level: </><Level>%d</>\n"
					"<Small>ManaCost: </><ManaCost>%.1f</>\n"

					"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
					
					"<Default>Launched %d bolts of fire, "
					"exploding on impact and dealing: </>"
					"<Damage>%d</><Default> fire damage with a chance to burn</>\n\n"),
			CurrentLevel,
			ManaCost,
			Cooldown,
			FMath::Min(MaxNumProjectiles, CurrentLevel),
			ScaledDamage);
	}
}

FString UAuraFireBolt::GetNextLevelDescription(int32 CurrentLevel) const
{
	const int32 NextLevel = CurrentLevel + 1;
	const int32 ScaledDamage = Damage.GetValueAtLevel(CurrentLevel);
	const float ManaCost = GetManaCost(NextLevel);
	const float Cooldown = GetCooldown(NextLevel);
	return FString::Printf(
			TEXT("<Title>NEXT LEVEL</>\n\n"
			
					"<Small>Level: </><Level>%d</>\n"
					"<Small>ManaCost: </><ManaCost>%.1f</>\n"

					"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
					
					"<Default>Launched %d bolts of fire, "
					"exploding on impact and dealing: </>"
					"<Damage>%d</><Default> fire damage with a chance to burn</>\n\n"),
			NextLevel,
			ManaCost,
			Cooldown,
			FMath::Min(MaxNumProjectiles, NextLevel),
			ScaledDamage);
}

void UAuraFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocketTag,
	bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	if(!GetAvatarActorFromActorInfo()->HasAuthority()) return;

	if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const int32 NumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
		//const int32 NumProjectiles = MaxNumProjectiles;
		if(NumProjectiles > 1)
		{
			const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
			FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
			if(bOverridePitch)
			{
				Rotation.Pitch = PitchOverride;
			}
		
			const FVector Forward = Rotation.Vector();

			TArray<FRotator> Rotators = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, NumProjectiles);

			for (const auto& Rot : Rotators)
			{
				FTransform SpawnTransform;
				SpawnTransform.SetLocation(SocketLocation);
				SpawnTransform.SetRotation(Rot.Quaternion());

				const auto SpawnedProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
					ProjectileClass,
					SpawnTransform,
					GetAvatarActorFromActorInfo(),
					Cast<APawn>(GetOwningActorFromActorInfo()),
					ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		
				SpawnedProjectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

				if(HomingTarget && HomingTarget->Implements<UCombatInterface>())
				{
					SpawnedProjectile->GetMovementComponent()->HomingTargetComponent = HomingTarget->GetRootComponent();
				}
				else
				{
					SpawnedProjectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
					SpawnedProjectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
					SpawnedProjectile->GetMovementComponent()->HomingTargetComponent = SpawnedProjectile->HomingTargetSceneComponent;
				}

				SpawnedProjectile->GetMovementComponent()->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
				SpawnedProjectile->GetMovementComponent()->bIsHomingProjectile = bLaunchHomingProjectile;
				
				SpawnedProjectile->FinishSpawning(SpawnTransform);
			} 
		}
		else
		{
			//Single Projectile
			SpawnProjectile(ProjectileTargetLocation, SocketTag, bOverridePitch, PitchOverride);
		}
	}
}
