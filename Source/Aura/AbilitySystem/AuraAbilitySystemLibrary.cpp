// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Data/CharacterClassInfo.h"
#include "Game/AuraGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if(!WorldContextObject) return nullptr;

	if(const auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			
			return AuraHUD->GetOverlayWidgetController(FWidgetControllerParams{PC, PS, ASC, AS});
		}
	}
	
	return nullptr; 
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if(!WorldContextObject) return nullptr;

	if(const auto PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if(AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			
			return AuraHUD->GetAttributeMenuWidgetController(FWidgetControllerParams{PC, PS, ASC, AS});
		}
	}
	
	return nullptr; 
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass, float Level)
{
	const UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(!CharacterClassInfo|| !ASC) return;
	
	const FCharacterClassDefaultInfo& ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	const AActor* AvatarActor = ASC->GetAvatarActor();
	
	//Apply Primary Attributes
	FGameplayEffectContextHandle PrimaryEffectContext = ASC->MakeEffectContext();
	PrimaryEffectContext.AddSourceObject(AvatarActor);
	const auto PrimaryAttributesEffectSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryEffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesEffectSpecHandle.Data.Get());

	//Apply Secondary Attributes
	FGameplayEffectContextHandle SecondaryEffectContext = ASC->MakeEffectContext();
	SecondaryEffectContext.AddSourceObject(AvatarActor);
	const auto SecondaryAttributesEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level,SecondaryEffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesEffectSpecHandle.Data.Get());

	//Apply Vital Attributes
	FGameplayEffectContextHandle VitalEffectContext = ASC->MakeEffectContext();
	VitalEffectContext.AddSourceObject(AvatarActor);
	const auto VitalAttributesEffectSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalEffectContext);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesEffectSpecHandle.Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	const UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(!CharacterClassInfo || !ASC) return;

	for (const auto& AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		const auto AbilitySpec = FGameplayAbilitySpec{AbilityClass, 1};
		ASC->GiveAbility(AbilitySpec);
	} 
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(!AuraGameMode) return nullptr;

	return AuraGameMode->CharacterClassInfo;
}
