// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "AuraEffectActor.generated.h"

class UAbilitySystemComponent;
struct FActiveGameplayEffectHandle;
class UGameplayEffect;
class UStaticMeshComponent;
class USphereComponent;

UENUM(BlueprintType)
enum class EEffectApplicationPolicy : uint8
{
	ApplyOnOverlap,
	ApplyOnEndOverlap,
	DoNotApply,
};

UENUM(BlueprintType)
enum class EEffectRemovalPolicy : uint8
{
	RemoveOnEndOverlap,
	DoNotRemove,
};

USTRUCT()
struct FCurrentActiveEffectHandles
{
	GENERATED_BODY()

	TArray<FActiveGameplayEffectHandle> Handles;
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AAuraEffectActor();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TArray<TSubclassOf<UGameplayEffect>> InstantGameplayEffectClasses;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TArray<TSubclassOf<UGameplayEffect>> DurationGameplayEffectClasses;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy DurationApplicationPolicy = EEffectApplicationPolicy::DoNotApply;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	TArray<TSubclassOf<UGameplayEffect>> InfiniteGameplayEffectClasses;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectApplicationPolicy InfiniteApplicationPolicy = EEffectApplicationPolicy::DoNotApply;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	EEffectRemovalPolicy InfiniteRemovalPolicy = EEffectRemovalPolicy::RemoveOnEndOverlap;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Applied Effects")
	float ActorLevel = 1.f;
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, meta=(BlueprintProtected))
	void ApplyEffectToTarget(AActor* TargetActor, const TArray<TSubclassOf<UGameplayEffect>>& GameplayEffectSubclasses);
	UFUNCTION(BlueprintCallable, meta=(BlueprintProtected))
	void OnOverlap(AActor* TargetActor);
	UFUNCTION(BlueprintCallable, meta=(BlueprintProtected))
	void EndOverlap(AActor* TargetActor);


private:
	
	UFUNCTION()
	void OnTargetActorDestroyed(AActor* DestroyedActor);
	UPROPERTY()
	TMap<UAbilitySystemComponent*, FCurrentActiveEffectHandles> ActiveEffectHandles;
};
