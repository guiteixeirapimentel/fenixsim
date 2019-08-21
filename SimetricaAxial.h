#pragma once
#include "LinAlg.h"

class SimetricaAxial
{
public:
	SimetricaAxial(double densidade, double rugosidade, double altura);
	~SimetricaAxial();

	virtual double PegarPosVCp() const;
	virtual double PegarPosVCM() const;
	virtual double PegarVolumeAparente() const;
	virtual double PegarVolumeReal() const;
	virtual double PegarMassa() const;
	virtual double PegarMomentoInerciaX() const;

	virtual double PegarCNAlpha(double alpha, double AREAREF) const = 0;
	virtual double PegarCMAlpha(double alpha, double AREAREF, double COMPREF) const = 0;
	virtual double PegarCDampin(const VETOR& velInf, const VETOR& velAng, double AREAREF, double COMPREF) const = 0;
	virtual double PegarCdAtrit(double reFog, double mach, double alturaFoguete, double finenessRatioRock, double AREAREF) const = 0;
	virtual double PegarCdPress(double mach, double AREAREF) const = 0;


	virtual double PegarCdPressBase(double mach, double AREAREF) const = 0;

	virtual double PegarAltura() const;
	virtual double PegarRaioBaseInt() const;
	virtual double PegarRaioTopoInt() const;
	virtual double PegarRaioBaseExt() const;
	virtual double PegarRaioTopoExt() const;

	virtual double PegarDensidade() const;
	virtual double PegarRugosidade() const;
protected:
	double cDensidade;
	double cAltura;
	double cVolumeAparente;
	double cVolumeReal;
	double cAreaLateral;
	double cMassa;
	double cMomentoInerciaX;
	double cRugosidade;
	double cPosVCP;
	double cPosVCM;
};