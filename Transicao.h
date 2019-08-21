#pragma once
#include "SimetricaAxial.h"

class Transicao : public SimetricaAxial
{
public:
	Transicao(double densidade, double rugosidade, double altura, double raioIntTopo, double raioExtTopo, 
		double raioIntBase, double raioExtBase);
	~Transicao();

	virtual double PegarCNAlpha(double alpha, double AREAREF) const override;
	virtual double PegarCMAlpha(double alpha, double AREAREF, double COMPREF) const override;
	virtual double PegarCDampin(const VETOR& velInf, const VETOR& velAng, double AREAREF, double COMPREF) const override;
	virtual double PegarCdAtrit(double reFog, double mach, double alturaFoguete, double finenessRatioRock, double AREAREF) const override;

	virtual double PegarCdPressBase(double mach, double AREAREF) const override;

	double PegarRaioBaseInt() const override;
	double PegarRaioTopoInt() const override;
	double PegarRaioBaseExt() const override;
	double PegarRaioTopoExt() const override;


protected:
	double cRaioExternoTopo;
	double cRaioInternoTopo;
	double cRaioExternoBase;
	double cRaioInternoBase;
	
	double cAreaTransversalTopo;
	double cAreaTransversalBase;
};