#include "AumentoRaio.h"
#include "Conica.h"

AumentoRaio::AumentoRaio(double densidade, double rugosidade, double altura, double raioIntTopo, double raioExtTopo,
	double raioIntBase, double raioExtBase)
	:
	Transicao(densidade, rugosidade, altura, raioIntTopo, raioExtTopo, raioIntBase, raioExtBase)
{
	cThetaTransicao = atan((cRaioExternoBase - cRaioExternoBase) / (2.0 * cAltura));
	
	const double alturaConeMaior = (cAltura * cRaioExternoBase) / (cRaioExternoBase - cRaioExternoTopo);
	const double alturaConeMenor = alturaConeMaior - altura;

	const Conica coneMaior(densidade, rugosidade, alturaConeMaior, raioIntBase, raioExtBase);
	const Conica coneMenor(densidade, rugosidade, alturaConeMenor, raioIntTopo, raioExtTopo);

	cMomentoInerciaX = coneMaior.PegarMomentoInerciaX() - coneMenor.PegarMomentoInerciaX();
	
}

AumentoRaio::~AumentoRaio()
{}

double AumentoRaio::PegarCdPress(double mach, double AREAREF) const
{
	double cdPress = 0.8 * pow(sin(cThetaTransicao), 2);

	cdPress *= cAreaTransversalBase - cAreaTransversalTopo;

	return cdPress;
}