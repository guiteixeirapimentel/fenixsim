#include "Conica.h"

Conica::Conica(double densidade, double rugosidade, double altura, double raioBaseInt, double raioBaseExt)
	:
	Coifa(densidade, rugosidade, altura, raioBaseInt, raioBaseExt),
	cTheta(atan(raioBaseExt / altura))
{
	cVolumeAparente = M_PI * cRaioBaseExterno * cRaioBaseExterno * cAltura / 3.0;
	double espessura = cRaioBaseExterno - cRaioBaseInterno;
	const double volInt = (M_PI * cRaioBaseInterno * cRaioBaseInterno * (cAltura - espessura) / 3.0);

	cVolumeReal = cVolumeAparente - volInt;

	cMassa = cVolumeReal * cDensidade;

	cAreaLateral = M_PI * cRaioBaseExterno * sqrt(pow(cRaioBaseExterno, 2) + pow(cAltura, 2));

	cPosVCM = 3.0 * cAltura / 4.0f;

	cPosVCP = ((cAltura * cAreaBase) - cVolumeAparente) / (cAreaBase - 0.0);

	cMomentoInerciaX = (3.0 / 20.0) * cMassa * (pow(cRaioBaseExterno, 2) + (4.0 * pow(cAltura, 2)));
	//cMomentoInerciaX -= (3.0 / 20.0) * (volInt * cDensidade) * (pow(cRaioBaseInterno, 2) + (4.0 * pow(cAltura - espessura, 2)));
}

Conica::~Conica()
{}

double Conica::PegarCDampin(const VETOR& velInf, const VETOR& velAng, double AREAREF, double COMPREF) const
{
	double cdamping = 0.55 *
		((pow(cAltura, 4) * cRaioBaseExterno) / (AREAREF * COMPREF))
		* (pow(Modulus(velAng), 2) / pow(Modulus(velInf), 2));

	return cdamping;
}

double Conica::PegarCdAtrit(double reFog, double mach, double alturaFoguete, double finenessRatioRock, double AREAREF) const
{
	double cfCoifa = 0.0;
	double cReynoldsCritico = 51.0 * pow(cRugosidade / alturaFoguete, -1.039);

	if (reFog < 1e4) // reynolds < 10^4
	{
		cfCoifa = 1.48e-2;
	}
	else if (reFog < cReynoldsCritico)
	{
		cfCoifa = 1.0 / pow((1.5*log(reFog)) - 5.6, 2);
	}
	else
	{
		cfCoifa = 0.032 * pow(cRugosidade / alturaFoguete, 0.2);
	}

	double corrComp = 1.0 - (0.1 * pow(mach, 2));
	cfCoifa *= corrComp;

	double cdFricCoifa = (1.0 + (1.0 / (2.0*finenessRatioRock))) * cAreaLateral * cfCoifa / AREAREF;

	return cdFricCoifa;
}

double Conica::PegarCdPress(double mach, double AREAREF) const
{
	double cdPressCoifa = 0.8 * pow(sin(cTheta), 2) * cAreaBase / AREAREF;

	return cdPressCoifa;
}