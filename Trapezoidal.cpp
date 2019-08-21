#include "Trapezoidal.h"

Trapezoidal::Trapezoidal(double densidade, double rugosidade, int numeroAletas, double envergadura, double espessura, PERFIL perfil,
	double posVertical, double cordaRaiz, double cordaPonta, double deslCordaPonta, double raioNaAleta)
	:
	Aleta(densidade, rugosidade, numeroAletas, envergadura, espessura, perfil, posVertical, raioNaAleta),
	cCordaPonta(cordaPonta),
	cCordaRaiz(cordaRaiz),
	cDeslCordaPonta(deslCordaPonta)
{
	cAreaLateral = (cCordaPonta + cCordaRaiz) * 0.5 * cEnvergadura;
	cVolume = cAreaLateral * cEspessura;
	
	cMassa = cVolume * cDensidade * cNumeroAletas;

	cPosVCP = ((cDeslCordaPonta / 3.0) *
		((cCordaRaiz + (2.0 * cCordaPonta)) / (cCordaPonta + cCordaRaiz)))
		+
		((cCordaRaiz + cCordaPonta - ((cCordaPonta*cCordaRaiz) /
		(cCordaRaiz + cCordaPonta))) / 6.0);

	cPosVCM = ((1.0f / (cCordaRaiz + cCordaPonta)) *
		(((pow(cDeslCordaPonta, 2) - pow(cCordaRaiz - cDeslCordaPonta - cCordaPonta, 2)) / 3.0)
			- (cCordaRaiz * (cDeslCordaPonta - cCordaRaiz))
			));

	cTauL = atan(cDeslCordaPonta / cEnvergadura);

	double compEnvAleta = sqrt((cEnvergadura*cEnvergadura) +
		pow((0.5*(cCordaPonta - cCordaRaiz) + cDeslCordaPonta), 2));

	cTauC = acos(cEnvergadura / compEnvAleta);

	VETOR P1 = {0.0, cCordaRaiz};
	VETOR P2 = {cEnvergadura, cDeslCordaPonta};
	VETOR P3 = {cEnvergadura, cDeslCordaPonta + cCordaPonta };
	VETOR P4 = { 0.0, 0.0};

	const double XCM = (2.0 * cEnvergadura / (cCordaRaiz + cCordaPonta)) * (((cCordaPonta - cCordaRaiz) / 3.0) + (0.5 * cCordaRaiz));

	const VETOR PCM = {XCM, cPosVCM};

	P1 -= PCM;
	P2 -= PCM;
	P3 -= PCM;
	P4 -= PCM;

	double topSum = Modulus(CrossProduct(P1, P2)) * (DotProduct(P1, P1) + DotProduct(P1, P2) + DotProduct(P2, P2));
	topSum += Modulus(CrossProduct(P2, P3)) * (DotProduct(P2, P2) + DotProduct(P2, P3) + DotProduct(P3, P3));
	topSum += Modulus(CrossProduct(P3, P4)) * (DotProduct(P3, P3) + DotProduct(P3, P4) + DotProduct(P4, P4));
	topSum += Modulus(CrossProduct(P4, P1)) * (DotProduct(P4, P4) + DotProduct(P4, P1) + DotProduct(P1, P1));

	double bottomSum = Modulus(CrossProduct(P1, P2)) + Modulus(CrossProduct(P2, P3)) +
		Modulus(CrossProduct(P3, P4)) + Modulus(CrossProduct(P4, P1));

	cMomentoInerciaX = cMassa * topSum / (6.0 * bottomSum);
}

Trapezoidal::~Trapezoidal()
{}

double Trapezoidal::PegarCNAlpha(double mach, double raioTuboNaAleta, double AREAREF) const
{
	double beta = sqrt(abs((mach * mach) - 1.0));

	double cnalphaAletas = 2.0 * M_PI * (pow(cEnvergadura, 2) / AREAREF)
		/ (1.0 + sqrt(
			1.0 + pow(beta * (cEnvergadura * cEnvergadura) / (cAreaLateral * cos(cTauC)), 2)
		));

	//CONSIDERANDO SEMPRE QUE OS FINS ESTÃO A 0 GRAU DO ESCOAMENTO LATERAL
	if (cNumeroAletas < 3)
	{
		cnalphaAletas = cnalphaAletas;
	}
	else if (cNumeroAletas == 3)
	{
		const double theta = 0.0;
		cnalphaAletas = cnalphaAletas * 1.5 * (1.0 - (0.15*pow(cos(3.0 / 2.0*theta), 2)));
	}
	else if (cNumeroAletas == 4)
	{
		const double theta = 0.0;
		cnalphaAletas = cnalphaAletas * 2.0 * (1.0 - (0.06*pow(sin(2.0*theta), 2)));
	}
	else if (cNumeroAletas == 5)
	{
		cnalphaAletas = cnalphaAletas * 2.37;
	}
	else if (cNumeroAletas == 6)
	{
		cnalphaAletas = cnalphaAletas * 2.74;
	}
	else if (cNumeroAletas == 7)
	{
		cnalphaAletas = cnalphaAletas * 2.99;
	}

	cnalphaAletas *= 1.0 + (raioTuboNaAleta / (cEnvergadura + raioTuboNaAleta));

	return cnalphaAletas;
}

double Trapezoidal::PegarCMAlpha(double alpha, double AREAREF, double COMPREF) const
{
	return 0.0;
}

double Trapezoidal::PegarCDampin(const VETOR& velInf, const VETOR& velAng, double distCM, double AREAREF, double COMPREF) const
{
	double cdampingAletas = 0.6 * (cAreaLateral * distCM * distCM / (AREAREF * COMPREF))
		*(pow(Modulus(velAng), 2) / pow(Modulus(velInf), 2));

	if (cNumeroAletas < 4)
	{
		cdampingAletas *= cNumeroAletas;
	}
	else
	{
		cdampingAletas *= 4.0;
	}

	return cdampingAletas;
}

double Trapezoidal::PegarCdAtrit(double reFog, double mach, double alturaFoguete, double finenessRatioRock, double AREAREF) const
{
	double cfAleta = 0.0;
	double reynoldsCritico = 51.0 * pow(cRugosidade / alturaFoguete, -1.039);

	if (reFog < 1e4) // reynolds < 10^4
	{
		cfAleta = 1.48e-2;
	}
	else if (reFog < reynoldsCritico)
	{
		cfAleta = 1.0 / pow((1.5*log(reFog)) - 5.6, 2);
	}
	else
	{
		cfAleta = 0.032 * pow(cRugosidade / alturaFoguete, 0.20);
	}

	double corrComp = 1.0 - (0.1 * pow(mach, 2));
	cfAleta *= corrComp;

	double relAfilamento = cCordaPonta / cCordaRaiz;
	double cbarra = (2.0 / 3.0) * cCordaRaiz * ((1.0 + relAfilamento + pow(relAfilamento, 2)) / (1.0 + relAfilamento));

	//parte da aleta
	double cdFricAleta = (1.0 + ((2.0 * cEspessura) / cbarra))
		* cAreaLateral* 2.0 * cNumeroAletas * cfAleta / AREAREF;

	return cdFricAleta;
}

double Trapezoidal::PegarCdPress(double mach, double AREAREF) const
{
	double cdPressAletasLE = 0.0;
	double finFrontalArea = 0.0;

	switch (cPerfil)
	{
	case RETANGULAR:
	{
		cdPressAletasLE = 0.85 * (1.0 + (pow(mach, 2) / 4.0) + (pow(mach, 4) / 40.0)) * pow(cos(cTauL), 2);

		finFrontalArea = cEspessura * cEnvergadura * cNumeroAletas;
	}
		break;
	case ARREDONDADO:
	{
		if (mach < 0.9)
		{
			cdPressAletasLE = pow(1.0 - pow(mach, 2), -0.417) - 1.0;
			cdPressAletasLE *= pow(cos(cTauL), 2);
		}
		else
		{
			cdPressAletasLE = 1.0 - (1.785*(mach - 0.9));
			cdPressAletasLE *= pow(cos(cTauL), 2);
		}

		finFrontalArea = (cEspessura / 2.0) * M_PI * cEnvergadura * cNumeroAletas;
	}
		break;

	default:
		cdPressAletasLE = 0.85 * (1.0 + (pow(mach, 2) / 4.0) + (pow(mach, 4) / 40.0)) * pow(cos(cTauL), 2);

		finFrontalArea = cEspessura * cEnvergadura * cNumeroAletas;
		break;
	}	

	double cdAletasBase = 0.0;//0.12 + (0.13 * pow(mach, 2));	

	double cdPresAletasTotal = cdPressAletasLE + cdAletasBase;
	
	return (cdPresAletasTotal * finFrontalArea / AREAREF);
}

double Trapezoidal::PegarCordaRaiz() const
{
	return cCordaRaiz;
}

double Trapezoidal::PegarCordaPonta() const
{
	return cCordaPonta;
}

double Trapezoidal::PegarDeslCordaPonta() const
{
	return cDeslCordaPonta;
}