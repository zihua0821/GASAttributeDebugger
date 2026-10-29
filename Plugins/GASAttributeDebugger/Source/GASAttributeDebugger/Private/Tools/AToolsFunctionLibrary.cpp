// Copyright (c) 2026 zihua0821. Licensed under the MIT License.


#include "Tools/AToolsFunctionLibrary.h"

#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "UObject/UnrealType.h"

static const TArray<FString> NetModeNames = {
	"Standalone",
	"DedicatedServer",
	"ListenServer",
	"Client",
	"MAX"
};

FString UAToolsFunctionLibrary::GetWorldDisplayNameWithNetMode(const UWorld* World, const int32 Index)
{
	return FString::Printf(TEXT("%s (%s %d)"), *World->GetName(), *NetModeNames[World->GetNetMode()], Index);
}

void UAToolsFunctionLibrary::GetAttributeInfo(const UAttributeSet* SrcAttributeSet, const FGameplayAttribute& Attribute, FString& AttributeName, float& AttributeValue)
{
	const bool bIsValid = SrcAttributeSet && Attribute.GetAttributeSetClass() == SrcAttributeSet->GetClass();
	if (!bIsValid)
	{
		return;
	}
	AttributeName = Attribute.GetName();
	AttributeValue = Attribute.GetNumericValue(SrcAttributeSet);
}

void UAToolsFunctionLibrary::ApplyEditorAttributeChange(UAbilitySystemComponent* ASC, const FGameplayAttribute& Attribute, const FString& ModType, const float Value)
{
	if (!ASC || !Attribute.IsValid()) { return; }
	UGameplayEffect* GameplayEffect = NewObject<UGameplayEffect>(GetTransientPackage(), TEXT("RuntimeInstantGE"));
	GameplayEffect->DurationPolicy = EGameplayEffectDurationType::Instant;
	FGameplayModifierInfo GameplayModifierInfo;
	GameplayModifierInfo.Attribute = Attribute;
	GameplayModifierInfo.ModifierMagnitude = FScalableFloat(Value);
	if (ModType == "Add (Base)")
	{
		GameplayModifierInfo.ModifierOp = EGameplayModOp::AddBase;
	}
	else if (ModType == "Multiply (Additive)")
	{
		GameplayModifierInfo.ModifierOp = EGameplayModOp::MultiplyAdditive;
	}
	else if (ModType == "Divide (Additive)")
	{
		GameplayModifierInfo.ModifierOp = EGameplayModOp::DivideAdditive;
	}
	else if (ModType == "Multiply (Compound)")
	{
		GameplayModifierInfo.ModifierOp = EGameplayModOp::MultiplyCompound;
	}
	else if (ModType == "Add (Final)")
	{
		GameplayModifierInfo.ModifierOp = EGameplayModOp::AddFinal;
	}
	else if (ModType == "Override")
	{
		GameplayModifierInfo.ModifierOp = EGameplayModOp::Override;
	}
	else if (ModType == "Additive (Old)")
	{
		GameplayModifierInfo.ModifierOp = EGameplayModOp::Additive;
	}
	else if (ModType == "Multiplicitive (Old)")
	{
		GameplayModifierInfo.ModifierOp = EGameplayModOp::Multiplicitive;
	}
	else if (ModType == "Division (Old)")
	{
		GameplayModifierInfo.ModifierOp = EGameplayModOp::Division;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Unknown ModType, falling back to AddBase"));
		GameplayModifierInfo.ModifierOp = EGameplayModOp::AddBase;
	}
	GameplayEffect->Modifiers.Add(GameplayModifierInfo);
	FGameplayEffectSpec GESpec(GameplayEffect, {}, 0.f);
	ASC->ApplyGameplayEffectSpecToSelf(GESpec);
}

TArray<UAttributeSet*> UAToolsFunctionLibrary::GetAllAttributeSets(const UAbilitySystemComponent* ASC)
{
	TArray<UAttributeSet*> AttributeSets;
	if (ASC)
	{
		for (const auto& Set : ASC->GetSpawnedAttributes())
		{
			if (Set)
			{
				AttributeSets.Add(Set);
			}
		}
	}
	return AttributeSets;
}

FString UAToolsFunctionLibrary::GetAttributeSetName(const UAttributeSet* AttributeSet)
{
	if (!AttributeSet)
	{
		return FString();
	}
	return AttributeSet->GetClass()->GetName();
}

TSubclassOf<UAttributeSet> UAToolsFunctionLibrary::GetAttributeSetClass(const UAttributeSet* AttributeSet)
{
	if (!AttributeSet)
	{
		return nullptr;
	}
	return AttributeSet->GetClass();
}

TArray<FGameplayAttribute> UAToolsFunctionLibrary::GetAllAttributesWithSet(UAbilitySystemComponent* ASC, const TSubclassOf<UAttributeSet>& AttributeSetClass)
{
	TArray<FGameplayAttribute> Attributes;
	if (ASC)
	{
		TArray<FGameplayAttribute> AllAttributes;
		ASC->GetAllAttributes(AllAttributes);
		for (const FGameplayAttribute& Attribute : AllAttributes)
		{
			if (Attribute.GetAttributeSetClass() == AttributeSetClass)
			{
				Attributes.Add(Attribute);
			}
		}
	}
	return Attributes;
}

bool UAToolsFunctionLibrary::CheckAttributeIsReplicated(const FGameplayAttribute& Attribute)
{
	const UClass* AttributeSetClass = Attribute.GetAttributeSetClass();
	// 遍历类的所有属性
	for (TFieldIterator<FProperty> PropIt(AttributeSetClass); PropIt; ++PropIt)
	{
		const FProperty* Property = *PropIt;
		if (Property->GetFName() == Attribute.AttributeName)
		{
			// 检查是否存在 Replicated 元数据标记
			const FString& RepMeta = Property->GetMetaData(TEXT("Replicated"));
			return RepMeta.Equals(TEXT("true"), ESearchCase::IgnoreCase) ||
					Property->HasAnyPropertyFlags(CPF_Net); // CPF_Net 是复制标志位
		}
	}
	return false;
}
