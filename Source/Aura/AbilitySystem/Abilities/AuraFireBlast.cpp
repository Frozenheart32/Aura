// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/AuraFireBlast.h"

#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Actor/AuraFireBall.h"

FString UAuraFireBlast::GetDescription(int32 CurrentLevel) const
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(CurrentLevel);
	const float ManaCost = GetManaCost(CurrentLevel);
	const float Cooldown = GetCooldown(CurrentLevel);
	if(CurrentLevel == 1)
	{
		
		return FString::Printf(
			TEXT("<Title>FIRE BLAST</>\n\n"
			
					"<Small>Level: </><Level>%d</>\n"
					
					"<Small>ManaCost: </><ManaCost>%.1f</>\n"

					"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
					
					"<Default>Launched a ball of fire, "
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
			TEXT("<Title>FIRE BLAST</>\n\n"
			
					"<Small>Level: </><Level>%d</>\n"
					"<Small>ManaCost: </><ManaCost>%.1f</>\n"

					"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
					
					"<Default>Launched %d , fire balls in all directions"
					"and exploding upon return, causing: </>"
					"<Damage>%d</><Default> fire damage with a chance to burn</>\n\n"),
			CurrentLevel,
			ManaCost,
			Cooldown,
			FMath::Min(NumFireBalls, CurrentLevel),
			ScaledDamage);
	}
}

FString UAuraFireBlast::GetNextLevelDescription(int32 CurrentLevel) const
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
					
					"<Default>Launched %d , fire balls in all directions"
					"and exploding upon return, causing: </>"
					"<Damage>%d</><Default> fire damage with a chance to burn</>\n\n"),
			NextLevel,
			ManaCost,
			Cooldown,
			FMath::Min(NumFireBalls, NextLevel),
			ScaledDamage);
}

TArray<AAuraFireBall*> UAuraFireBlast::SpawnFireBalls()
{
	TArray<AAuraFireBall*> SpawnedFireBalls{};

	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	//Get "Nova" directions
	const TArray<FRotator> FireBallDirections = UAuraAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, 360.f, NumFireBalls);

	for (const auto& Rotator : FireBallDirections)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		
		AAuraFireBall* FireBall = GetWorld()->SpawnActorDeferred<AAuraFireBall>(
			FireBallClass,
			SpawnTransform,
			GetAvatarActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		
		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		FireBall->ReturnToActor = GetAvatarActorFromActorInfo();

		FireBall->ExplosionDamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
		
		SpawnedFireBalls.Add(FireBall);

		FireBall->FinishSpawning(SpawnTransform);
	} 
	
	return SpawnedFireBalls;
}
