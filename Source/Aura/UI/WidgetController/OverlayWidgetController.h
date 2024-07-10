// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "Engine/DataTable.h"
#include "OverlayWidgetController.generated.h"

struct FAuraAbilityInfo;
class UAbilityInfo;
class UAuraMessageWidget;
class UAuraAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag AssetTag = FGameplayTag{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Message = FText{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraMessageWidget> MessageWidget;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangeSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, const FUIWidgetRow&, MessageData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangeSignature, int32, NewValue);

/**
 * 
 */
UCLASS()
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangeSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangeSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangeSignature OnManaChanged;
	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangeSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|AbilityInfo")
	FAbilityInfoSignature AbilityInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
	FOnAttributeChangeSignature OnXPPercentChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|XP")
	FOnPlayerStatChangeSignature OnPlayerLevelChanged;

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;

public:

	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

private:

	UFUNCTION()
	void OnXPChanged(int32 NewXP) const;
	UFUNCTION()
	void OnLevelChanged(int32 NewLevel) const;

	template<class T>
	const T* GetDataTableRowByTag(const UDataTable* DataTable, const FGameplayTag& Tag) const;

	UFUNCTION()
	void OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraASC);
};

template <class T>
const T* UOverlayWidgetController::GetDataTableRowByTag(const UDataTable* DataTable, const FGameplayTag& Tag) const
{
	return DataTable->FindRow<T>(Tag.GetTagName(), TEXT(""));
}
