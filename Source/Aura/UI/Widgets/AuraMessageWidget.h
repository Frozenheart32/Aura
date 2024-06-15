// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/AuraUserWidget.h"
#include "AuraMessageWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class AURA_API UAuraMessageWidget : public UAuraUserWidget
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(meta=(BindWidget))
	UTextBlock* TextBlock_Message = nullptr;
	UPROPERTY(meta=(BindWidget))
	UImage* Image_Icon = nullptr;

public:

	UFUNCTION(BlueprintCallable)
	void SetMessage(UTexture2D* NewIcon, const FText& NewText);
};
