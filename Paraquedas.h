#pragma once
#include "MassaPonto.h"

class Paraquedas : public MassaPonto
{
public:
	Paraquedas(double massa, double posVertical, double delayDeploy, double diametro, double cd);
	~Paraquedas();

	double PegarCd(double AREAREF) const;
	double PegarDelay() const;
	double PegarRaio() const;
	double PegarArea() const;
private:
	const double cDelay;
	const double cRaio;
	const double cCD;
	const double cArea;
};