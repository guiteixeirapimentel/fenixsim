#include "Coifa.h"
#define _USE_MATH_DEFINES
#include <cmath>

Coifa::Coifa(double densidade, double rugosidade, double altura, double raioBaseInt, double raioBaseExt)
	:
	SimetricaAxial(densidade, rugosidade, altura),
	cRaioBaseInterno(raioBaseInt),
	cRaioBaseExterno(raioBaseExt),
	cAreaBase(M_PI * cRaioBaseExterno * cRaioBaseExterno)
{}

Coifa::~Coifa()
{}

double Coifa::PegarCNAlpha(double alpha, double AREAREF) const
{
	double cnalpha = (2.0 / AREAREF) * (cAreaBase - 0.0);

	if (alpha != 0.0)
	{
		cnalpha *= sin(alpha) / alpha;
	}

	cnalpha += 1.1 * (cAreaLateral / AREAREF) * pow(sin(alpha), 2);

	return cnalpha;
}

double Coifa::PegarCMAlpha(double alpha, double AREAREF, double COMPREF) const
{
	double cmalpha = (2.0 / (AREAREF * COMPREF)) * ((cAltura*cAreaBase) - cVolumeAparente);

	if (alpha != 0.0)
	{
		cmalpha *= sin(alpha) / alpha;
	}
	return cmalpha;
}

double Coifa::PegarRaioBaseExt() const
{
	return cRaioBaseExterno;
}

double Coifa::PegarRaioBaseInt() const
{
	return cRaioBaseInterno;
}

double Coifa::PegarCdPressBase(double mach, double AREAREF) const
{
	double cdPressBaseTransicao = 0.12 + (0.13*pow(mach, 2));

	return cdPressBaseTransicao * cAreaBase / AREAREF;
}