// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBolt.h"

#include "Actor/AuraProjectile.h"
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
		const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		if(bOverridePitch)
		{
			Rotation.Pitch = PitchOverride;
		}
		
		const FVector Forward = Rotation.Vector();
		const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread / 2.f, FVector::UpVector);
		const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread / 2.f, FVector::UpVector);
		
		const int32 NumProjectiles = FMath::Min(MaxNumProjectiles, GetAbilityLevel());
		
		if(NumProjectiles > 1)
		{
			const float DeltaSpread = ProjectileSpread / (NumProjectiles - 1);
			for (int32 i = 0; i < NumProjectiles; i++)
			{
				const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
				UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Direction * 100.f,
			5.f, FLinearColor::Red, 120.f);
			}
		}
		else
		{
			//Single Projectile
			SpawnProjectile(ProjectileTargetLocation, SocketTag, bOverridePitch, PitchOverride);
		}

		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Forward * 100.f,
			5.f, FLinearColor::White, 120.f);

		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + LeftOfSpread * 100.f,
			1.f, FLinearColor::Gray, 120.f);
		UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + RightOfSpread * 100.f,
			1.f, FLinearColor::Gray, 120.f);

		
		/*
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());

		const auto SpawnedProjectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(
			ProjectileClass,
			SpawnTransform,
			GetAvatarActorFromActorInfo(),
			Cast<APawn>(GetOwningActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		
		SpawnedProjectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		SpawnedProjectile->FinishSpawning(SpawnTransform);
		*/
	}
}
