#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

class FunctionMath
{
public:
	virtual double operator()(double x) const = 0;

};