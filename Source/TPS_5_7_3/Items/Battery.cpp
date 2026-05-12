// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Battery.h"

using namespace TPS;

constexpr float ChargeAmount = 0.1f;

Battery::Battery(float PercentIn)
{
	SetPercent(PercentIn);
}

void Battery::Charge()
{
	SetPercent(Percent + ChargeAmount);
}

void Battery::UnCharge()
{
	SetPercent(Percent - ChargeAmount);
}

float Battery::GetPercent() const
{
	return Percent;
}

FColor Battery::GetColor() const
{
	if (Percent > 0.7f)
	{
		return FColor::Green;
	}
	else if (Percent > 0.2f)
	{
		return FColor::Yellow;
	}
	else
	{
		return FColor::Red;
	}
}

FString Battery::ToString() const
{
	return FString::Printf(TEXT("%i%%"), FMath::RoundToInt(Percent  * 100.0f));
}

void Battery::SetPercent(float PercentIn)
{
	Percent = FMath::Clamp(PercentIn, 0.0f, 1.0f);
}
