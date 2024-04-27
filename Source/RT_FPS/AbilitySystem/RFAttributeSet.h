// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "RFAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

// Deliver necessary information such as effect causer and victim, effect information, and effect values to the Deligate.
DECLARE_MULTICAST_DELEGATE_FourParams(FAttributeEvent, AActor* /*EffectInstigator*/, AActor* /*EffectCauser*/, const FGameplayEffectSpec& /*EffectSpec*/, float /*EffectMagnitude*/);

/**
 * 
 */
UCLASS()
class RT_FPS_API URFAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	URFAttributeSet();

	ATTRIBUTE_ACCESSORS(URFAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(URFAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(URFAttributeSet, Damage);

	FAttributeEvent OnOutOfHealth;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*
	* called just before you modify the property value to decide whether to cancel the modification.
	* For example, you can cancel the modification by checking the invincible status.
	* However, it is not called while the buff that is applied for a certain period of time, such as a persistent buff effect, is maintained.
	*/
	virtual bool PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data) override;

	/*
	* Called just before the gameplay effects run to modify the default values in the attributes.
	* No more changes can be made. Main function that modifies the value.
	*/
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/* 
	* A function called just before the default change occurs, 
	* which prevents it from exceeding the maximum health 
	* if someone intentionally try to enter a large amount of health.
	*/
	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	
	/*
	* Called just before the value is modified in a modifire-like manner, 
	* typically by clamping before a value such as damage is applied.
	*/
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	
	/*
	* Called just after any modification happens to an attribute.
	* The most common post-processing function.
	*/
	//virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldValue);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Health", Meta = (/*HideFromModifiers,*/ AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	UPROPERTY(BlueprintReadOnly, Category = "Health", Meta = (/*HideFromModifiers,*/ AllowPrivateAccess = true))
	FGameplayAttributeData Damage;

	UPROPERTY()
	uint8 bIsDeath : 1;
};
