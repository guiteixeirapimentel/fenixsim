#pragma once
#include "Aleta.h"

class Trapezoidal : public Aleta
{
public:
	Trapezoidal(double densidade, double rugosidade, int numeroAletas, double envergadura, double espessura, PERFIL perfil,
		double posVertical, double cordaRaiz, double cordaPonta, double deslCordaPonta, double raioNaAleta);
	~Trapezoidal();

	virtual double PegarCNAlpha(double mach, double raioTuboNaAleta, double AREAREF) const;
	virtual double PegarCMAlpha(double alpha, double AREAREF, double COMPREF) const;
	virtual double PegarCDampin(const VETOR& velInf, const VETOR& velAng, double distCM, double AREAREF, double COMPREF) const;
	virtual double PegarCdAtrit(double reFog, double mach, double alturaFoguete, double finenessRatioRock, double AREAREF) const;
	virtual double PegarCdPress(double mach, double AREAREF) const;

	virtual double PegarCordaRaiz() const override;
	virtual double PegarCordaPonta() const override;
	virtual double PegarDeslCordaPonta() const override;
private:
	double cCordaPonta;
	double cCordaRaiz;
	double cDeslCordaPonta;
};