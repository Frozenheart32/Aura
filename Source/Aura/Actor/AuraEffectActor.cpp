// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"

AAuraEffectActor::AAuraEffectActor()
{
 	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("SceneRoot"));
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, const TArray<TSubclassOf<UGameplayEffect>>& GameplayEffectSubclasses)
{
	const bool bIsEnemy = TargetActor->ActorHasTag(FName{"Enemy"});
	if(bIsEnemy && !bApplyEffectsToEnemies) return;
	
	check(GameplayEffectSubclasses.Num() > 0);
	
	if(const auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor))
	{
		TArray<FActiveGameplayEffectHandle> Effects;
		for (const auto& EffectSubclass : GameplayEffectSubclasses)
		{
			FGameplayEffectContextHandle ContextHandle = TargetASC->MakeEffectContext();
			ContextHandle.AddSourceObject(this);

			const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(EffectSubclass, ActorLevel, ContextHandle);
			const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

			const bool bIsInfinite  = EffectSpecHandle.Data.Get()->Def->DurationPolicy == EGameplayEffectDurationType::Infinite;
			if(bIsInfinite && InfiniteRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
			{
				Effects.Add(ActiveEffectHandle);
			}
		}

		ActiveEffectHandles.Add(TargetASC, FCurrentActiveEffectHandles{Effects});
	}
}

void AAuraEffectActor::OnOverlap(AActor* TargetActor)
{
	const bool bIsEnemy = TargetActor->ActorHasTag(FName{"Enemy"});
	if(bIsEnemy && !bApplyEffectsToEnemies) return;
	
	if(InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClasses);
	}

	if(DurationApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClasses);
	}

	if(InfiniteApplicationPolicy == EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClasses);
		TargetActor->OnDestroyed.AddDynamic(this, &AAuraEffectActor::OnTargetActorDestroyed);
	}

	if(bDestroyOnEffectApplication &&
		(InstantGameplayEffectClasses.Num() > 0 || DurationGameplayEffectClasses.Num() > 0) 
		&& InfiniteGameplayEffectClasses.Num() == 0)
	{
		Destroy();
	}
}

void AAuraEffectActor::EndOverlap(AActor* TargetActor)
{
	const bool bIsEnemy = TargetActor->ActorHasTag(FName{"Enemy"});
	if(bIsEnemy && !bApplyEffectsToEnemies) return;
	
	//Apply effect
	if(InstantEffectApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InstantGameplayEffectClasses);
	}

	if(DurationApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, DurationGameplayEffectClasses);
	}

	if(InfiniteApplicationPolicy == EEffectApplicationPolicy::ApplyOnEndOverlap)
	{
		ApplyEffectToTarget(TargetActor, InfiniteGameplayEffectClasses);
	}

	
	//Remove effect
	if(InfiniteRemovalPolicy == EEffectRemovalPolicy::RemoveOnEndOverlap)
	{
		TargetActor->OnDestroyed.RemoveDynamic(this, &AAuraEffectActor::OnTargetActorDestroyed);
		
		UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if(!IsValid(TargetASC)) return;
		
		if(ActiveEffectHandles.Contains(TargetASC))
		{
			const FCurrentActiveEffectHandles* HandlesToRemove = ActiveEffectHandles.Find(TargetASC);

			for (const auto& Handle : HandlesToRemove->Handles)
			{
				TargetASC->RemoveActiveGameplayEffect(Handle, 1);
			} 
			
			ActiveEffectHandles.Remove(TargetASC);
		}
	}
}

void AAuraEffectActor::OnTargetActorDestroyed(AActor* DestroyedActor)
{
	if(const auto ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(DestroyedActor))
	{
		if(ActiveEffectHandles.Contains(ASC))
		{
			const FCurrentActiveEffectHandles* HandlesToRemove = ActiveEffectHandles.Find(ASC);

			for (const auto& Handle : HandlesToRemove->Handles)
			{
				ASC->RemoveActiveGameplayEffect(Handle);
			} 
			
			ActiveEffectHandles.Remove(ASC);
		}
	}

	DestroyedActor->OnDestroyed.RemoveDynamic(this, &AAuraEffectActor::OnTargetActorDestroyed);
}

