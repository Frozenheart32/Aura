// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS(Abstract)
class AURA_API UAuraWidgetController : public UObject
{
	GENERATED_BODY()

public:

	
protected:
	
	UPROPERTY()
	TWeakObjectPtr<APlayerController> PlayerController;
	UPROPERTY()
	TWeakObjectPtr<APlayerState> PlayerState;

	UPROPERTY()
	TWeakObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	UPROPERTY()
	TWeakObjectPtr<UAttributeSet> AttributeSet;

public:

	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	APlayerController* GetOwningPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	APlayerState* GetOwningPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	UAbilitySystemComponent* GetOwningASC() const;

	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	UAttributeSet* GetOwningAttributeSet() const;
};
