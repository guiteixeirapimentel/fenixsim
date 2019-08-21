#pragma once
#include "LinAlg.h"

class Aleta
{
public:
	enum PERFIL
	{
		RETANGULAR,
		ARREDONDADO
	};

public:
	Aleta(double densidade, double rugosidade, int numeroAletas, double envergadura, double espessura, PERFIL perfil, double posVertical, double raioNaAleta);
	~Aleta();

	virtual double PegarPosVCp() const;
	virtual double PegarPosVCM() const;
	virtual double PegarVolume() const;
	virtual double PegarMassa() const;
	virtual double PegarMomentoInerciaX() const;
	virtual double PegarPosV() const;
	virtual double PegarRaioNaAleta() const;
	
	virtual double PegarCNAlpha(double mach, double raioTuboNaAleta, double AREAREF) const = 0;
	virtual double PegarCMAlpha(double alpha, double AREAREF, double COMPREF) const = 0;
	virtual double PegarCDampin(const VETOR& velInf, const VETOR& velAng, double distCM, double AREAREF, double COMPREF) const = 0;
	virtual double PegarCdAtrit(double reFog, double mach, double alturaFoguete, double finenessRatioRock, double AREAREF) const = 0;
	virtual double PegarCdPress(double mach, double AREAREF) const = 0;

	virtual double PegarEnvergadura() const;
	virtual double PegarEspessura() const;

	virtual double PegarCordaRaiz() const;
	virtual double PegarCordaPonta() const;
	virtual double PegarDeslCordaPonta() const;
	
	virtual int PegarNumAletas() const;

	virtual void AlterarRaioNaAleta(double novoRaio);

	double PegarDensidade() const;
	double PegarRugosidade() const;
protected:
	double cDensidade;
	double cEnvergadura;
	double cEspessura;
	double cVolume;
	double cAreaLateral;
	double cMassa;
	double cMomentoInerciaX;
	double cRugosidade;
	double cPosVCP;
	double cPosVCM;

	int cNumeroAletas;

	double cTauL;
	double cTauC;

	double cPosV;
	double cRaioNaAleta;

	PERFIL cPerfil;
};