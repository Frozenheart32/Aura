// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class UAuraInputConfig;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class IEnemyInterface;

/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	AAuraPlayerController();

	virtual void PlayerTick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	TWeakInterfacePtr<IEnemyInterface> LastActor;
	TWeakInterfacePtr<IEnemyInterface> ThisActor;

	UFUNCTION()
	void Move(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void CursorTrace();

	UFUNCTION()
	void AbilityInputTagPressed(FGameplayTag InputTag);
	UFUNCTION()
	void AbilityInputTagReleased(FGameplayTag InputTag);
	UFUNCTION()
	void AbilityInputTagHeld(FGameplayTag InputTag);
};
