#pragma once
#include "Coifa.h"

class Conica : public Coifa
{
public:
	Conica(double densidade, double rugosidade, double altura, double raioBaseInt, double raioBaseExt);
	~Conica();

	virtual double PegarCDampin(const VETOR& velInf, const VETOR& velAng, double AREAREF, double COMPREF) const override;
	virtual double PegarCdAtrit(double reFog, double mach, double alturaFoguete, double finenessRatioRock, double AREAREF) const override;
	virtual double PegarCdPress(double mach, double AREAREF) const override;
private:
	double cTheta;
};