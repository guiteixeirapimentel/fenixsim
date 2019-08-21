#define _USE_MATH_DEFINES
#include <cmath>
#include "Paraquedas.h"

Paraquedas::Paraquedas(double massa, double posVertical, double delayDeploy, double diametro, double cd)
	:
	MassaPonto(massa, posVertical),
	cDelay(delayDeploy),
	cRaio(diametro / 2.0),
	cCD(cd),
	cArea(cRaio * cRaio * M_PI)
{}

Paraquedas::~Paraquedas()
{}

double Paraquedas::PegarCd(double AREAREF) const
{
	return cCD * cArea / AREAREF;
}

double Paraquedas::PegarDelay() const
{
	return cDelay;
}

double Paraquedas::PegarRaio() const
{
	return cRaio;
}

double Paraquedas::PegarArea() const
{
	return cArea;
}