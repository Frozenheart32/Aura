// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 CurrentLevel) const
{
	const int32 ScaledDamage = Damage.GetValueAtLevel(CurrentLevel);
	const float ManaCost = GetManaCost(CurrentLevel);
	const float Cooldown = GetCooldown(CurrentLevel);
	if(CurrentLevel == 1)
	{
		
		return FString::Printf(
			TEXT("<Title>ELECTROCUTE</>\n\n"
			
					"<Small>Level: </><Level>%d</>\n"
					
					"<Small>ManaCost: </><ManaCost>%.1f</>\n"

					"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
					
					"<Default>Emits a beam of lightning, "
					"connecting with the target, repeately causing: </>"
					"<Damage>%d</><Default> lighting damage with a chance to stun</>\n\n"),
			CurrentLevel,
			ManaCost,
			Cooldown,
			ScaledDamage);
	}	
	else
	{
		return FString::Printf(
			TEXT("<Title>ELECTROCUTE</>\n\n"
			
					"<Small>Level: </><Level>%d</>\n"
					"<Small>ManaCost: </><ManaCost>%.1f</>\n"

					"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
					
					"<Default>Emits a beam of lightinig, "
					"propagating to %d addtional targets nearby, causing: </>"
					"<Damage>%d</><Default> lightining damage with a chance to stun</>\n\n"),
			CurrentLevel,
			ManaCost,
			Cooldown,
			FMath::Min(MaxNumShockTargets - 1, CurrentLevel),
			ScaledDamage);
	}
}

FString UElectrocute::GetNextLevelDescription(int32 CurrentLevel) const
{
	const int32 NextLevel = CurrentLevel + 1;
	const int32 ScaledDamage = Damage.GetValueAtLevel(NextLevel);
	const float ManaCost = GetManaCost(NextLevel);
	const float Cooldown = GetCooldown(NextLevel);
	
	return FString::Printf(
			TEXT("<Title>NEXT LEVEL</>\n\n"
			
					"<Small>Level: </><Level>%d</>\n"
					"<Small>ManaCost: </><ManaCost>%.1f</>\n"

					"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"
					
					"<Default>Emits a beam of lightinig, "
					"propagating to %d addtional targets nearby, causing: </>"
					"<Damage>%d</><Default> lightining damage with a chance to stun</>\n\n"),
			NextLevel,
			ManaCost,
			Cooldown,
			FMath::Min(MaxNumShockTargets - 1, NextLevel),
			ScaledDamage);
}
