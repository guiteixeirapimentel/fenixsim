#include "Integration.h"
#include <assert.h>
#include <math.h>

void SimpsonUsage()
{
	const int m = 6;
	const double a = 0;
	const double b = 3.14159265359;
	const double h = (b - a) / m;

	double* y = new double[m + 1];
	for (int i = 0; i < m + 1; i++)
	{
		double x = (i * h) + a;
		y[i] = exp(x) + sin(x) + 2;
	}

	double saida = SimpsonOneThird(y, h, m + 1);

	delete[]y;
}

double SimpsonOneThird(const double* pY, double h, const unsigned int m, double* pOutErr)
{
	if (pOutErr != nullptr)
	{

	}

	if ((m % 2) != 1)
	{
		assert(0);
		return -0.0;
	}

	double integral4 = 0.0;
	double integralT = 0.0;
	for (unsigned int i = 0; i < m - 1; i += 2)
	{
		integral4 += (pY[i + 1]);

		integralT += pY[i] + pY[i + 2];
	}

	integral4 *= 4.0;
	
	integralT += integral4;

	integralT *= h / 3;

	return integralT;
}

int PesAbsGL(int numPontos, double** pOutPesos, double** pOutAbscissas)
{
	if (numPontos < 1)
	{
		return 0;
	}

	const int m = int(0.5 * (numPontos + 1));

	*pOutPesos = new double[m];
	*pOutAbscissas = new double[m];

	for (int i = 1; i <= m; i++)
	{
		double z = cos(M_PI * (i - 0.25) / (numPontos + 0.5));

		double difAbs = 1.0;
		double pp = 0.0;

		while (difAbs > 10e-15)
		{
			double p1 = 1.0;
			double p2 = 0.0;

			for (int j = 1; j <= numPontos; j++)
			{
				double p3 = p2;
				p2 = p1;
				//Polinomio de Legendre no ponto Z
				p1 = ((2.0 * j - 1.0)*z*p2 - (j - 1.0)*p3) / j;
			}
			//Derivada de Legendre no ponto z
			pp = numPontos * ((z * p1) - p2) / ((z*z) - 1.0);
			double z1 = z;

			//Metodo de Newton para calcular os zeros do polinomio

			z = z1 - p1 / pp;
			difAbs = abs(z - z1);
		}

		(*pOutAbscissas)[m - i] = z; // Abscissa
		(*pOutPesos)[m - i] = 2.0 / ((1.0 - (z*z))*pp*pp); // peso
		//somente raizes nao negativas são calculadas devido à simetria
	}

	return m;
}

double GaussLegendreIntegration(double a, double b, int numPontos, const FunctionMath& fx)
{
	double integral = 0.0;

	double* pPesosGL = nullptr;
	double* pAbscGL = nullptr;
	const int numPesos = PesAbsGL(numPontos, &pPesosGL, &pAbscGL);

	if (!numPesos)
	{
		return -0.0;
	}

	//Calculo da Integral

	double e1 = (b - a) / 2.0;
	double e2 = (a + b) / 2.0;

	double c1 = 0.0;
	double c2 = 0.0;

	if ((numPontos % 2) == 0)
	{
		c1 = 1.0;
		c2 = 0.5;
	}
	else
	{
		c1 = 0.0;
		c2 = 1.0;
	}

	for (int i = 1; i <= numPontos; i++)
	{
		const int k = int(i - (0.5 * (numPontos + 1)) + (sgn(i - (0.5 * (numPontos + c1)))) * c2);

		double t = sgn(k) * pAbscGL[abs(k) - 1];
		double x = (e1 * t) + e2;

		double y = fx(x);

		double c = pPesosGL[abs(k) - 1];

		integral += y * c;
	}

	integral *= e1;

	if (pPesosGL)
	{
		delete[] pPesosGL;
		pPesosGL = nullptr;
	}
	if (pAbscGL)
	{
		delete[] pAbscGL;
		pAbscGL = nullptr;
	}

	return integral;
}

double GaussLegendreIterIntegration(double a, double b, const FunctionMath& fx, double tolerancia, int numPtosMax, double* pOutpErr)
{
	double integral = 0.0;
	double delta = 10e100;

	int i = 0;
	int n1 = 5;
	int n2 = 8;
	int n = 0;

	double integral2 = GaussLegendreIntegration(a, b, n2, fx);
	
	while (!(delta <= tolerancia || n >= numPtosMax))
	{
		i += 1;
		n = n1 + n2;

		integral = GaussLegendreIntegration(a, b, n, fx);
		if (abs(integral) > 10e-15)
		{
			delta = abs((integral - integral2) / integral);
		}
		else
		{
			delta = abs(integral - integral2);
		}

		integral2 = integral;
		n1 = n2;
		n2 = n;
	}

	*pOutpErr = delta;

	return integral;
}