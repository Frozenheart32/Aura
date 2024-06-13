// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "UObject/NoExportTypes.h"
#include "AuraWidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FWidgetControllerParams
{
	GENERATED_BODY()

	FWidgetControllerParams() = default;
	
	FWidgetControllerParams(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
		: PlayerController(PC), PlayerState(PS), AbilitySystemComponent(ASC), AttributeSet(AS)
	{
			
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerController> PlayerController = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<APlayerState> PlayerState = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UAttributeSet> AttributeSet = nullptr;
	
};

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable)
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

	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FWidgetControllerParams& WCParams);
	virtual void BroadcastInitialValues();
	virtual void BindCallbacksToDependencies();

	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	APlayerController* GetOwningPlayerController() const;

	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	APlayerState* GetOwningPlayerState() const;

	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	UAbilitySystemComponent* GetOwningASC() const;

	UFUNCTION(BlueprintCallable, Category = "WidgetController")
	UAttributeSet* GetOwningAttributeSet() const;
};
