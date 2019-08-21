#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "FunctionMath.h"

//Retorna sinal do numero passado
template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

double SimpsonOneThird(const double* pY, double deltaXh, unsigned int nPoints, double* pOutErr = nullptr);

//NÂO ESQUECER, FUNÇÂO ALOCA MEMORIA, NECESSARIO DESALOCAR(VETORES) POUTPESOS E POUTABSCISSAS
int PesAbsGL(int numPontos, double** pOutPesos, double** pOutAbscissas);

double GaussLegendreIntegration(double a, double b, int numPontos, const FunctionMath& fx);

double GaussLegendreIterIntegration(double a, double b, const FunctionMath& fx, double tolerancia, int numPtosMax, double* pOutpErr);