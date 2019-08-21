#include "Transicao.h"

Transicao::Transicao(double densidade, double rugosidade, double altura, double raioIntTopo, double raioExtTopo,
	double raioIntBase, double raioExtBase)
	:
	SimetricaAxial(densidade, rugosidade, altura),
	cRaioInternoTopo(raioIntTopo),
	cRaioExternoTopo(raioExtTopo),
	cRaioInternoBase(raioIntBase),
	cRaioExternoBase(raioExtBase)
{
	cAreaTransversalTopo = M_PI * cRaioExternoTopo * cRaioExternoTopo;
	cAreaTransversalBase = M_PI * cRaioExternoBase * cRaioExternoBase;

	const double rext = std::fmin(cRaioExternoBase, cRaioExternoTopo);
	const double Rext = std::fmax(cRaioExternoBase, cRaioExternoTopo);

	double rint = std::fmin(cRaioInternoBase, cRaioInternoTopo);
	double Rint = std::fmax(cRaioInternoBase, cRaioInternoTopo);

	cVolumeAparente = (cAltura * M_PI / 3.0) * (pow(Rext, 2) + (Rext * rext) + pow(rext,2));

	cVolumeReal = cVolumeAparente - (cAltura * M_PI / 3.0) * (pow(Rint, 2) + (Rint * rint) + pow(rint, 2));

	cMassa = cVolumeReal * cDensidade;

	cPosVCM = (2*cAltura)/(rext + Rext) * (
		((1.0/3.0)*(rext - Rext)) + (Rext/2.0)
		);

	cPosVCP = ((cAltura * cAreaTransversalBase) - cVolumeAparente) / (cAreaTransversalBase - cAreaTransversalTopo);

	cAreaLateral = M_PI*(Rext + rext) * sqrt(pow(Rext - rext, 2) + pow(cAltura, 2));
}

Transicao::~Transicao()
{}

double Transicao::PegarCNAlpha(double alpha, double AREAREF) const
{
	double cnalphaTransicao = (2.0 / AREAREF)*(cAreaTransversalBase - cAreaTransversalTopo);
	if (alpha != 0.0)
	{
		cnalphaTransicao *= sin(alpha) / alpha;
	}
	cnalphaTransicao += 1.1 * (cAreaLateral / AREAREF) * pow(sin(alpha), 2);

	return cnalphaTransicao;
}

double Transicao::PegarCMAlpha(double alpha, double AREAREF, double COMPREF) const
{
	double cmalphaTransicao = (2.0 / (AREAREF * COMPREF)) * ((cAltura*cAreaTransversalBase) - cVolumeAparente);

	if (alpha != 0.0)
	{
		cmalphaTransicao *= sin(alpha) / alpha;
	}

	return cmalphaTransicao;
}

double Transicao::PegarCDampin(const VETOR& velInf, const VETOR& velAng, double AREAREF, double COMPREF) const
{
	double cdampingTransicao = 0.55 *
		((pow(cAltura, 4) * (cRaioExternoBase + cRaioExternoTopo) * 0.5) / (AREAREF * COMPREF))
		* (pow(Modulus(velAng), 2) / pow(Modulus(velInf), 2));

	return cdampingTransicao;
}

double Transicao::PegarCdAtrit(double reFog, double mach, double alturaFoguete, double finenessRatioRock, double AREAREF) const
{
	double cfCorpo = 0.0;
	double cReynoldsCritico = 51.0 * pow(cRugosidade / alturaFoguete, -1.039);

	if (reFog < 1e4) // reynolds < 10^4
	{
		cfCorpo = 1.48e-2;
	}
	else if (reFog < cReynoldsCritico)
	{
		cfCorpo = 1.0 / pow((1.5*log(reFog)) - 5.6, 2);
	}
	else
	{
		cfCorpo = 0.032 * pow(cRugosidade / alturaFoguete, 0.2);
	}

	double corrComp = 1.0 - (0.1 * pow(mach, 2));
	cfCorpo *= corrComp;

	double cdFricCorp = (1.0 + (1.0 / (2.0*finenessRatioRock))) * cAreaLateral * cfCorpo / AREAREF;

	return cdFricCorp;
}

double Transicao::PegarRaioBaseExt() const
{
	return cRaioExternoBase;
}
double Transicao::PegarRaioTopoExt() const
{
	return cRaioExternoTopo;
}

double Transicao::PegarRaioBaseInt() const
{
	return cRaioInternoBase;
}
double Transicao::PegarRaioTopoInt() const
{
	return cRaioInternoTopo;
}

double Transicao::PegarCdPressBase(double mach, double AREAREF) const
{
	double cdPressBaseTransicao = 0.12 + (0.13*pow(mach, 2));

	return cdPressBaseTransicao * cAreaTransversalBase / AREAREF;
}