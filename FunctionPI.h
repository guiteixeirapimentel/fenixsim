#pragma once
#include "FunctionMath.h"

class FunctionPI : public FunctionMath
{
public:
	double operator()(double x) const override
	{
		return 4.0 / (1.0 + (x*x));
	}
};