#pragma once
#include "FunctionMath.h"

class FunctionErr : public FunctionMath
{
public:
	double operator() (double x) const override
	{
		return exp(-x*x/2.0)/sqrt(2.0*3.14159);
	}
};