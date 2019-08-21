#pragma once
#include "FunctionMath.h"

class FunctionSin : public FunctionMath
{
	double operator()(double x) const override
	{
		return sin(x);
	}
};