// Copyright (c) 2026 zihua0821. Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeLineHandle.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChanged, float, NewValue);
/**
 * 
 */
UCLASS(BlueprintType)
class GASATTRIBUTEDEBUGGER_API UAttributeLineHandle : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void Bind(UAbilitySystemComponent* ASC, const FGameplayAttribute& Attribute);
	
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChanged OnAttributeChanged;

	UFUNCTION(BlueprintCallable)
	void Destroy();
private:
	void AttributeChanged(const FOnAttributeChangeData& Data) const;
};
