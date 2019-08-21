#pragma once
#include "FunctionMath.h"

class FunctionCos : public FunctionMath
{
public:
	double operator() (double theta) const override
	{
		return cos(theta);
	}
};