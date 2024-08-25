// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

class AMagicCircle;
class UNiagaraSystem;
class UDamageTextComponent;
class USplineComponent;
class UAuraInputConfig;
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;
class IEnemyInterface;
class UAuraAbilitySystemComponent;

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

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle();
	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();

protected:

	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;


public:

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

private:

	UPROPERTY(EditDefaultsOnly, Category = "Floating Text")
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	UPROPERTY()
	mutable TWeakObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;
	bool bShiftKeyDown = false;
	
	TWeakInterfacePtr<IEnemyInterface> LastActor;
	TWeakInterfacePtr<IEnemyInterface> ThisActor;
	FHitResult CursorHit;

	UFUNCTION()
	void Move(const FInputActionValue& InputActionValue);
	UFUNCTION()
	void ShiftPressed() { bShiftKeyDown = true; }
	UFUNCTION()
	void ShiftReleased() { bShiftKeyDown = false; }

	UFUNCTION()
	void CursorTrace();

	UFUNCTION()
	void AbilityInputTagPressed(FGameplayTag InputTag);
	UFUNCTION()
	void AbilityInputTagReleased(FGameplayTag InputTag);
	UFUNCTION()
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UAuraAbilitySystemComponent* GetASC() const;
	void AutoRun();

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;

	void UpdateMagicCircleLocation();
};
