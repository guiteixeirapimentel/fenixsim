#pragma once
#include "Transicao.h"

class AumentoRaio : public Transicao
{
public:
	AumentoRaio(double densidade, double rugosidade, double altura, double raioIntTopo, double raioExtTopo,
		double raioIntBase, double raioExtBase);
	~AumentoRaio();

	virtual double PegarCdPress(double mach, double AREAREF) const override;

private:
	double cThetaTransicao;

};