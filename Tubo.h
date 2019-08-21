#pragma once
#include "SimetricaAxial.h"

class Tubo : public SimetricaAxial
{
public:
	Tubo(double densidade, double rugosidade, double altura, double raioInt, double raioExt);
	~Tubo();

	virtual double PegarCNAlpha(double alpha, double AREAREF) const;
	virtual double PegarCMAlpha(double alpha, double AREAREF, double COMPREF) const;
	virtual double PegarCDampin(const VETOR& velInf, const VETOR& velAng, double AREAREF, double COMPREF) const;
	virtual double PegarCdAtrit(double reFog, double mach, double alturaFoguete, double finenessRatioRock, double AREAREF) const;
	virtual double PegarCdPress(double mach, double AREAREF) const;

	virtual double PegarCdPressBase(double mach, double AREAREF) const override;

	double PegarRaioBaseInt() const override;
	double PegarRaioTopoInt() const override;
	double PegarRaioBaseExt() const override;
	double PegarRaioTopoExt() const override;
private:
	double cRaioExterno;
	double cRaioInterno;
	double cAreaTransversal;
};