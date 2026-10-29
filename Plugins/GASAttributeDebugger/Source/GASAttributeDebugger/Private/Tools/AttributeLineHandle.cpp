// Copyright (c) 2026 zihua0821. Licensed under the MIT License.


#include "Tools/AttributeLineHandle.h"

void UAttributeLineHandle::Bind(UAbilitySystemComponent* ASC, const FGameplayAttribute& Attribute)
{
	AddToRoot();
	if (ASC)
	{
		ASC->GetGameplayAttributeValueChangeDelegate(Attribute).AddUObject(this, &UAttributeLineHandle::AttributeChanged);
	}
}

void UAttributeLineHandle::Destroy()
{
	RemoveFromRoot();
}

void UAttributeLineHandle::AttributeChanged(const FOnAttributeChangeData& Data) const
{
	if (OnAttributeChanged.IsBound())
	{
		OnAttributeChanged.Broadcast(Data.NewValue);
	}
}
