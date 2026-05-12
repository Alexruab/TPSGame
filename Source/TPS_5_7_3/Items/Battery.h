// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

namespace TPS
{
class TPS_5_7_3_API Battery
{
public:
	Battery() = default;
	Battery(float PercentIn);
	
	void Charge();
	void UnCharge();
	
	float GetPercent() const;
	FColor GetColor() const;
	FString ToString() const;
	auto operator<=>(const Battery& Battery) const = default;
	
private:
	float Percent{1.0f};
	void SetPercent(float PercentIn);
};
} // namespace TPS