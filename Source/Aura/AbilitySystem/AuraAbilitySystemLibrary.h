// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystemLibrary.generated.h"

struct FDamageEffectParams;
class UAbilityInfo;
class AAuraHUD;
struct FGameplayEffectContextHandle;
class UCharacterClassInfo;
class UAbilitySystemComponent;
enum class ECharacterClass : uint8;
class UAttributeMenuWidgetController;
class UOverlayWidgetController;
class USpellMenuWidgetController;
struct FWidgetControllerParams;

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (WorldContext = "WorldContextObject"))
	static bool TryGetWidgetControllerParams(FWidgetControllerParams& OutParams, AAuraHUD*& OutAuraHUD, const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "AuraAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"),  Category = "AuraAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"),  Category = "AuraAbilitySystemLibrary|WidgetController")
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);


	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|ClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static void InitializeDefaultAttributes(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass, float Level);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults", meta = (WorldContext = "WorldContextObject"))
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AuraAbilitySystemLibrary|AbilityInfo", meta = (WorldContext = "WorldContextObject"))
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static float GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static FGameplayTag GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle);


	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsSuccessfulDebuff(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDamage(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDamage);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffDuration(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDebuffFrequency(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetDamageType(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType);


	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static void GetLivePlayersWithRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorsToIgnore, float Radius, const FVector& SphereOrigin);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayMechanics")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

	UFUNCTION(BlueprintCallable, BlueprintPure, meta = (WorldContext = "WorldContextObject"))
	static int32 GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "AuraAbilitySystemLibrary|DamageEffect")
	static FGameplayEffectContextHandle ApplyDamageEffect(const FDamageEffectParams& Params);
};
