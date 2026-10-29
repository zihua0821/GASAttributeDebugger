// Copyright (c) 2026 zihua0821. Licensed under the MIT License.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AToolsFunctionLibrary.generated.h"

DECLARE_DYNAMIC_DELEGATE_OneParam(FOnAttributeChange, float, Value);

class UAbilitySystemComponent;
class UAttributeSet;
struct FGameplayAttribute;
/**
 * 
 */
UCLASS()
class GASATTRIBUTEDEBUGGER_API UAToolsFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/*
	 * World
	 */

	// Get the display name of the world with net mode
	UFUNCTION(BlueprintPure, Category = "World")
	static FString GetWorldDisplayNameWithNetMode(const UWorld* World, int32 Index);

	/*
	 * AttributeSet
	 */

	// Get the attribute name and value
	UFUNCTION(BlueprintPure, Category = "AttributeSet")
	static void GetAttributeInfo(const UAttributeSet* SrcAttributeSet, const FGameplayAttribute& Attribute, FString& AttributeName, float& AttributeValue);

	// Apply the attribute change
	UFUNCTION(BlueprintCallable, Category = "AttributeSet")
	static void ApplyEditorAttributeChange(UAbilitySystemComponent* ASC, const FGameplayAttribute& Attribute, const FString& ModType, const float Value);

	UFUNCTION(BlueprintPure, Category = "AttributeSet")
	static TArray<UAttributeSet*> GetAllAttributeSets(const UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintPure, Category = "AttributeSet")
	static FString GetAttributeSetName(const UAttributeSet* AttributeSet);
	
	UFUNCTION(BlueprintPure, Category = "AttributeSet")
	static TSubclassOf<UAttributeSet> GetAttributeSetClass(const UAttributeSet* AttributeSet);
	
	UFUNCTION(BlueprintPure, Category = "AttributeSet")
	static TArray<FGameplayAttribute> GetAllAttributesWithSet(UAbilitySystemComponent* ASC, const TSubclassOf<UAttributeSet>& AttributeSetClass);

	UFUNCTION(BlueprintPure, Category = "AttributeSet")
	static bool CheckAttributeIsReplicated(const FGameplayAttribute& Attribute);
};

