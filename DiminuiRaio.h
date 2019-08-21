#pragma once
#include "Transicao.h"

class DiminuiRaio : public Transicao
{
public:
	DiminuiRaio(double densidade, double rugosidade, double altura, double raioIntTopo, double raioExtTopo,
		double raioIntBase, double raioExtBase);
	~DiminuiRaio();

	virtual double PegarCdPress(double mach, double AREAREF) const override;

private:
	double cGama;
	double cGamaMultiplier;
};