// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class AAuraAIController;
class UBehaviorTree;
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:

	AAuraEnemy();

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangeSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangeSignature OnMaxHealthChanged;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Combat")
	bool bHitReacting = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float BaseWalkSpeed = 250.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float LifeSpan = 5.f;

	UPROPERTY(BlueprintReadWrite, Category = "Combat")
	TObjectPtr<AActor> CombatTarget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> HealthBar;


	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	UPROPERTY()
	TWeakObjectPtr<AAuraAIController> AuraAIController;

	virtual void BeginPlay() override;

public:

	virtual void PossessedBy(AController* NewController) override;

	/* Enemy Interface*/
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;

	/* Combat Interface */
	virtual int32 GetPlayerLevel() const override;
	virtual void Die() override;

protected:

	virtual void InitAbilityActorInfo() override;
	virtual void InitializeDefaultAttributes() const override;

	void HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount);
};
