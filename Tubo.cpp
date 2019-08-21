#include "Tubo.h"

Tubo::Tubo(double densidade, double rugosidade, double altura, double raioInt, double raioExt)
	:
	SimetricaAxial(densidade, rugosidade, altura),
	cRaioInterno(raioInt),
	cRaioExterno(raioExt)
{
	cAreaTransversal = M_PI * cRaioExterno * cRaioExterno;

	cVolumeAparente = cAreaTransversal * cAltura;

	cVolumeReal = cVolumeAparente - (M_PI * cRaioInterno * cRaioInterno * cAltura);

	cMassa = cVolumeReal * cDensidade;

	cPosVCM = cAltura / 2.0;

	cPosVCP = cAltura / 2.0;

	cAreaLateral = 2.0 * M_PI * cRaioExterno *cAltura;

	cMomentoInerciaX = (1.0 / 12.0) * cMassa  * ( (3.0 * (pow(cRaioExterno, 2) + pow(cRaioInterno, 2))) + pow(cAltura, 2) );
}

Tubo::~Tubo()
{}

double Tubo::PegarCNAlpha(double alpha, double AREAREF) const
{	
	double cnalphaCorpo = 1.1 * (cAreaLateral / AREAREF) * pow(sin(alpha), 2);

	return cnalphaCorpo;
}

double Tubo::PegarCMAlpha(double alpha, double AREAREF, double COMPREF) const
{
	double cmalphaCorpo = (2.0 / (AREAREF * COMPREF)) * ((cAltura*cAreaTransversal) - cVolumeAparente);

	if (alpha != 0.0)
	{
		cmalphaCorpo *= sin(alpha) / alpha;
	}

	return cmalphaCorpo;
}

double Tubo::PegarCDampin(const VETOR& velInf, const VETOR& velAng, double AREAREF, double COMPREF) const
{
	double cdampingCorpo = 0.55 *
		((pow(cAltura, 4) * cRaioExterno) / (AREAREF * COMPREF))
		* (pow(Modulus(velAng), 2) / pow(Modulus(velInf), 2));

	return cdampingCorpo;
}

double Tubo::PegarCdAtrit(double reFog, double mach, double alturaFoguete, double finenessRatioRock, double AREAREF) const
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

double Tubo::PegarCdPress(double mach, double AREAREF) const
{
	return 0.0;
}

double Tubo::PegarCdPressBase(double mach, double AREAREF) const
{
	double cdPressBaseCorpo = 0.12 + (0.13*pow(mach, 2));

	return cdPressBaseCorpo * cAreaTransversal / AREAREF;
}

double Tubo::PegarRaioBaseExt() const
{
	return cRaioExterno;
}
double Tubo::PegarRaioTopoExt() const
{
	return cRaioExterno;
}
double Tubo::PegarRaioBaseInt() const
{
	return cRaioInterno;
}
double Tubo::PegarRaioTopoInt() const
{
	return cRaioInterno;
}