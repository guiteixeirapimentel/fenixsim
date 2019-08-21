#pragma once
#include "SimetricaAxial.h"

class Coifa : public SimetricaAxial
{
public:
	Coifa(double densidade, double rugosidade, double altura, double raioBaseInt, double raioBaseExt);
	~Coifa();

	virtual double PegarCNAlpha(double alpha, double AREAREF) const override;
	virtual double PegarCMAlpha(double alpha, double AREAREF, double COMPREF) const override;
	double PegarCdPressBase(double mach, double AREAREF) const override;

	double PegarRaioBaseInt() const override;
	double PegarRaioBaseExt() const override;
protected:
	double cRaioBaseInterno;
	double cRaioBaseExterno;
	double cAreaBase;
};