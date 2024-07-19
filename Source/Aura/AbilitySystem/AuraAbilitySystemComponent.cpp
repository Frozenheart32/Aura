// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraAbilitySystemLibrary.h"
#include "AuraGameplayTags.h"
#include "Abilities/AuraGameplayAbility.h"
#include "Data/AbilityInfo.h"
#include "Interaction/PlayerInterface.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied_OnClient);
	
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	const auto Tags = FAuraGameplayTags::Get();
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
		if(const auto AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			AbilitySpec.DynamicAbilityTags.AddTag(Tags.Abilities_Status_Equipped);
			GiveAbility(AbilitySpec);
		}
	}

	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast();
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(
	const TArray<TSubclassOf<UGameplayAbility>>& PassiveAbilities)
{
	for (const TSubclassOf<UGameplayAbility>& AbilityClass : PassiveAbilities)
	{
		FGameplayAbilitySpec AbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return;

	ABILITYLIST_SCOPE_LOCK();

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputReleased, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid()) return;
	
	ABILITYLIST_SCOPE_LOCK();

	for (auto& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			//For Wait press event
			InvokeReplicatedEvent(EAbilityGenericReplicatedEvent::InputPressed, AbilitySpec.Handle, AbilitySpec.ActivationInfo.GetActivationPredictionKey());
			if(!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock AbilityListLock{*this};
	for(const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if(!Delegate.ExecuteIfBound(AbilitySpec))
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to execute delegate in %hs"), __FUNCTION__);
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec) const
{
	if(AbilitySpec.Ability)
	{
		for(const FGameplayTag& Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName{"Abilities"})))
			{
				return Tag;
			}
		}
	}

	return FGameplayTag{};
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagForSpec(const FGameplayAbilitySpec& AbilitySpec) const
{
	for(const FGameplayTag& Tag : AbilitySpec.DynamicAbilityTags)
	{
		if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName{"InputTag"})))
		{
			return Tag;
		}
	}

	return FGameplayTag{};
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if(const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetInputTagForSpec(*Spec);
	}

	return FGameplayTag{};
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	const FGameplayTag CheckTag = FGameplayTag::RequestGameplayTag(FName{"Abilities.Status"});
	for(const FGameplayTag& StatusTag : AbilitySpec.DynamicAbilityTags)
	{
		if(StatusTag.MatchesTag(CheckTag))
		{
			return StatusTag;
		}
	}
	
	return FGameplayTag{};
}

FGameplayTag UAuraAbilitySystemComponent::GetStatusFromAbilityTag(const FGameplayTag& AbilityTag)
{
	if(const FGameplayAbilitySpec* Spec = GetSpecFromAbilityTag(AbilityTag))
	{
		return GetStatusFromSpec(*Spec);
	}

	return FGameplayTag{};
}

FGameplayAbilitySpec* UAuraAbilitySystemComponent::GetSpecFromAbilityTag(const FGameplayTag& AbilityTag)
{
	FScopedAbilityListLock ActiveScopeLoc{*this};
	auto& ActiveAbilities = GetActivatableAbilities();
	for (auto& AbilitySpec : ActiveAbilities)
	{
		for(const FGameplayTag& Tag : AbilitySpec.Ability.Get()->AbilityTags)
		{
			if(Tag.MatchesTag(AbilityTag))
			{
				return &AbilitySpec;
			}
		}
	}

	return nullptr;
}

void UAuraAbilitySystemComponent::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	if(GetAvatarActor()->Implements<UPlayerInterface>())
	{
		if(IPlayerInterface::Execute_GetAttributePoints(GetAvatarActor()) > 0)
		{
			UpgradeAttribute_OnServer(AttributeTag);
		}
	}
}

void UAuraAbilitySystemComponent::UpdateAbilityStatuses(int32 InLevel)
{
	const auto& AuraTags = FAuraGameplayTags::Get();
	
	const auto AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	for (const auto& Info : AbilityInfo->GetAbilityInformation())
	{
		if(!Info.AbilityTag.IsValid() || InLevel < Info.LevelRequirement) continue;
		
		if(FGameplayAbilitySpec* AbilitySpecPtr = GetSpecFromAbilityTag(Info.AbilityTag))
		{
			
		}
		else
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec{Info.AbilityClass, 1};
			AbilitySpec.DynamicAbilityTags.AddTag(AuraTags.Abilities_Status_Eligible);
			GiveAbility(AbilitySpec);
			MarkAbilitySpecDirty(AbilitySpec);
			UpdateAbilityStatus_OnClient(Info.AbilityTag, AuraTags.Abilities_Status_Eligible, 1);
		}
	} 
}

void UAuraAbilitySystemComponent::EquipAbility_OnServer_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& SlotTag)
{
	if(FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		const auto& AuraTags = FAuraGameplayTags::Get();
		const FGameplayTag PrevSlot = GetInputTagForSpec(*AbilitySpec);
		const FGameplayTag Status = GetStatusFromSpec(*AbilitySpec);

		const bool bStatusValid = Status == AuraTags.Abilities_Status_Equipped || Status == AuraTags.Abilities_Status_Locked;
		if(bStatusValid)
		{
			//Remove this input (slot) from any ability that has it
			ClearAbilitiesOfSlot(SlotTag);

			//clear this abilities slot, just in case, it's a different slot
			ClearSlot(AbilitySpec);
			//Now, Assing this ability to this slot
			AbilitySpec->DynamicAbilityTags.AddTag(SlotTag);
			if(Status.MatchesTagExact(AuraTags.Abilities_Status_Unlocked))
			{
				AbilitySpec->DynamicAbilityTags.RemoveTag(AuraTags.Abilities_Status_Unlocked);
				AbilitySpec->DynamicAbilityTags.AddTag(AuraTags.Abilities_Status_Equipped);
			}

			MarkAbilitySpecDirty(*AbilitySpec);
		}

		EquipAbility_OnClient(AbilityTag, AuraTags.Abilities_Status_Equipped, SlotTag, PrevSlot);
	}
}

void UAuraAbilitySystemComponent::EquipAbility_OnClient_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PreviousSlot)
{
	AbilityEquipped.Broadcast(AbilityTag, Status, Slot, PreviousSlot);
}

bool UAuraAbilitySystemComponent::GetDescriptionsByAbilityTag(FString& OutDescription, FString& OutNextLevelDescription,
                                                              const FGameplayTag& AbilityTag)
{
	if(const FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if(const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec->Ability))
		{
			OutDescription = AuraAbility->GetDescription(AbilitySpec->Level);
			OutNextLevelDescription = AuraAbility->GetNextLevelDescription(AbilitySpec->Level);
			return true;
		}
	}

	const UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(GetAvatarActor());
	check(AbilityInfo);

	const auto& AuraTags = FAuraGameplayTags::Get();
	if(!AbilityTag.IsValid() || AbilityTag.MatchesTagExact(AuraTags.Abilities_None))
	{
		OutDescription = FString{};
	}
	else
	{
		OutDescription = UAuraGameplayAbility::GetLockedDescription(AbilityInfo->FindAbilityInfoByTag(AbilityTag).LevelRequirement);
	}
	
	OutNextLevelDescription = FString{};
	return false;
}

void UAuraAbilitySystemComponent::ClearSlot(FGameplayAbilitySpec* Spec)
{
	const FGameplayTag Slot = GetInputTagForSpec(*Spec);
	Spec->DynamicAbilityTags.RemoveTag(Slot);
	MarkAbilitySpecDirty(*Spec);
}

void UAuraAbilitySystemComponent::ClearAbilitiesOfSlot(const FGameplayTag& Slot)
{
	FScopedAbilityListLock ActiveScopeLock{*this};
	for (FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if(AbilityHasSlot(&Spec, Slot))
		{
			ClearSlot(&Spec);
		}
	}
}

bool UAuraAbilitySystemComponent::AbilityHasSlot(FGameplayAbilitySpec* Spec, const FGameplayTag& SlotTag) const
{
	for (const FGameplayTag& Tag : Spec->DynamicAbilityTags)
	{
		if(Tag.MatchesTagExact(SlotTag))
			return true;
	}

	return false;
}

void UAuraAbilitySystemComponent::SpendSpellPoint_OnServer_Implementation(const FGameplayTag& AbilityTag)
{
	if(FGameplayAbilitySpec* AbilitySpec = GetSpecFromAbilityTag(AbilityTag))
	{
		if(GetAvatarActor() && GetAvatarActor()->Implements<UPlayerInterface>())
		{
			IPlayerInterface::Execute_AddToSpellPoints(GetAvatarActor(), -1);
		}
		
		const auto& AuraTags = FAuraGameplayTags::Get();
		FGameplayTag Status = GetStatusFromSpec(*AbilitySpec);
		
		if(Status.MatchesTagExact(AuraTags.Abilities_Status_Eligible))
		{
			AbilitySpec->DynamicAbilityTags.RemoveTag(Status);
			
			//Change status - Unlocked
			AbilitySpec->DynamicAbilityTags.AddTag(AuraTags.Abilities_Status_Unlocked);
			Status = AuraTags.Abilities_Status_Unlocked;
		}
		else if(Status.MatchesTagExact(AuraTags.Abilities_Status_Equipped) ||
			Status.MatchesTagExact(AuraTags.Abilities_Status_Unlocked))
		{
			AbilitySpec->Level += 1;
		}

		UpdateAbilityStatus_OnClient(AbilityTag, Status, AbilitySpec->Level);
		MarkAbilitySpecDirty(*AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::UpgradeAttribute_OnServer_Implementation(FGameplayTag AttributeTag)
{
	FGameplayEventData Payload;
	Payload.EventTag = AttributeTag;
	Payload.EventMagnitude = 1.f;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetAvatarActor(), AttributeTag, MoveTemp(Payload));

	if(GetAvatarActor()->Implements<UPlayerInterface>())
	{
		IPlayerInterface::Execute_AddToAttributePoints(GetAvatarActor(), -1);
	}
}

void UAuraAbilitySystemComponent::UpdateAbilityStatus_OnClient_Implementation(const FGameplayTag& AbilityTag,
	const FGameplayTag& StatusTag, int32 AbilityLevel)
{
	AbilityStatusChanged.Broadcast(AbilityTag, StatusTag, AbilityLevel);
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if(!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast();
	}
}

void UAuraAbilitySystemComponent::EffectApplied_OnClient_Implementation(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& EffectSpec,
                                                                        FActiveGameplayEffectHandle EffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
