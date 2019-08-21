#include "DiminuiRaio.h"
#include "Conica.h"

DiminuiRaio::DiminuiRaio(double densidade, double rugosidade, double altura, double raioIntTopo, double raioExtTopo,
	double raioIntBase, double raioExtBase)
	:
	Transicao(densidade, rugosidade, altura, raioIntTopo, raioExtTopo, raioIntBase, raioExtBase)
{
	cGama = cAltura / (2.0*(cRaioExternoTopo - cRaioExternoBase));

	if (cGama < 1.0)
	{
		cGamaMultiplier = 1.0;
	}
	else if (cGama < 3.0)
	{
		cGamaMultiplier = (3.0 - cGama) / 2.0;
	}
	else
	{
		cGamaMultiplier = 0;
	}

	const double alturaConeMaior = (cAltura * cRaioExternoBase) / (cRaioExternoTopo - cRaioExternoBase);
	const double alturaConeMenor = alturaConeMaior - altura;

	const Conica coneMaior(densidade, rugosidade, alturaConeMaior, raioIntBase, raioExtBase);
	const Conica coneMenor(densidade, rugosidade, alturaConeMenor, raioIntTopo, raioExtTopo);

	cMomentoInerciaX = coneMaior.PegarMomentoInerciaX() - coneMenor.PegarMomentoInerciaX();
}

DiminuiRaio::~DiminuiRaio()
{}

double DiminuiRaio::PegarCdPress(double mach, double AREAREF) const
{
	double cdPress = (cAreaTransversalBase/cAreaTransversalTopo);
	cdPress *= 0.12 + (0.13 * mach * mach);

	cdPress *= cGamaMultiplier;

	return cdPress;
}