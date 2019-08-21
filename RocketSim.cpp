#define _USE_MATH_DEFINES
#include "RocketSim.h"
#include <cassert>
#include <cmath>
#include <DirectXMath.h>

RocketSim::RocketSim(const RocketSimDesc& desc)
	:
	cRD(desc),
	cPSimulacao(nullptr)
{}

RocketSim::~RocketSim()
{
	if (cRD.cPMotor)
	{
		delete cRD.cPMotor;
		cRD.cPMotor = nullptr;
	}
	if (cPSimulacao)
	{
		delete cPSimulacao;
		cPSimulacao = nullptr;
	}
}

void RocketSim::Simular(double maxTime, double deltaTime)
{
	LimparAntigaSimulacao();

	cPSimulacao = new RocketSimData();
	cPSimulacao->cDeltaTime = deltaTime;

	const size_t numIteracoes = static_cast<size_t>(maxTime / deltaTime) + 1;

	cPSimulacao->cAceleracoes.reserve(numIteracoes);
	cPSimulacao->cAceleracoes.push_back(cRD.cAcelInicial);

	cPSimulacao->cVelocidades.reserve(numIteracoes);
	cPSimulacao->cVelocidades.push_back(cRD.cVelInicial);

	cPSimulacao->cPosicoes.reserve(numIteracoes);
	cPSimulacao->cPosicoes.push_back(cRD.cPosInicial);

	cPSimulacao->cAtitudes.reserve(numIteracoes);
	cPSimulacao->cAtitudes.push_back(cRD.cAtiInicial);

	cPSimulacao->cCds.reserve(numIteracoes);
	cPSimulacao->cCds.push_back(0.0);
		
	cPSimulacao->cCNs.reserve(numIteracoes);
	cPSimulacao->cCNs.push_back(0.0);

	cPSimulacao->cCMs.reserve(numIteracoes);
	cPSimulacao->cCMs.push_back(0.0);
	
	cPSimulacao->cMachs.reserve(numIteracoes);
	cPSimulacao->cMachs.push_back(0.0);

	cPSimulacao->cVelocidadesAngulares.reserve(numIteracoes);
	cPSimulacao->cVelocidadesAngulares.push_back({});

	cPSimulacao->cAceleracoesAngulares.reserve(numIteracoes);
	cPSimulacao->cAceleracoesAngulares.push_back({});

	cPSimulacao->cAlphas.reserve(numIteracoes);
	cPSimulacao->cAlphas.push_back(0.0);
	
	double tempoSimulacao = 0.0;

	const double delayParaquedas = cRD.cPFogueteData->cPParaquedas->PegarDelay();

	while (tempoSimulacao < delayParaquedas && tempoSimulacao < maxTime)
	{
		TickDefault(tempoSimulacao);
		tempoSimulacao += deltaTime;
	}

	while(tempoSimulacao < maxTime)
	{
		TickParaquedas(tempoSimulacao);
		tempoSimulacao += deltaTime;
	}

	cPSimulacao->cRodouSimulacao = true;
	cPSimulacao->cTempoTotal = tempoSimulacao;
	
	double altMax = 0.0;
	for (size_t i = 0; i < cPSimulacao->cPosicoes.size(); i++)
	{
		if (altMax < cPSimulacao->cPosicoes[i].y)
		{
			altMax = cPSimulacao->cPosicoes[i].y;
		}
	}

	cPSimulacao->cAltitudeMax = altMax;

	double velMax = 0.0;
	for (size_t i = 0; i < cPSimulacao->cVelocidades.size(); i++)
	{
		const double vel = ModulusSquared(cPSimulacao->cVelocidades[i]);
		if (velMax < vel)
		{
			velMax = vel;
		}
	}

	cPSimulacao->cVelocidadeMax = sqrt(velMax);

	double acelMax = 0.0;
	for (size_t i = 0; i < cPSimulacao->cAceleracoes.size(); i++)
	{
		const double acel = ModulusSquared(cPSimulacao->cAceleracoes[i]);
		if (acelMax < acel)
		{
			acelMax = acel;
		}
	}

	cPSimulacao->cAceleracaoMax = sqrt(acelMax);

	double alphaMax = 0.0;
	for (size_t i = 0; i < cPSimulacao->cAlphas.size(); i++)
	{
		const double alpha = cPSimulacao->cAlphas[i];

		if (abs(alphaMax) < abs(alpha))
		{
			alphaMax = alpha;
		}
	}

	cPSimulacao->cAlphaMaximo = alphaMax;
}


void RocketSim::TickDefault(double tempoAgora)
{
	VETOR vel = cPSimulacao->cVelocidades.back();
	VETOR acel = cPSimulacao->cAceleracoes.back();
	VETOR pos = cPSimulacao->cPosicoes.back();
	VETOR ati = cPSimulacao->cAtitudes.back();
	VETOR omega = cPSimulacao->cVelocidadesAngulares.back();

	if (!cSaiuHaste && Modulus(pos) > cRD.cComprimentoHaste)
	{
		cSaiuHaste = true;
	}

	cIntStdAtmosphere.SetAltitude(pos.y + cRD.cAltitudeLocal, cRD.cTemperaturaLocal);
	const double densidadeAr = cIntStdAtmosphere.GetAirDensity();
	const double velSomAr = cIntStdAtmosphere.GetSoundVelocity();
	const double viscAr = cIntStdAtmosphere.GetAirViscosity();
	
	const double massaAgora = cRD.cPFogueteData->PegarMassaFoguete(tempoAgora);
	
	VETOR velInf = vel - VETOR({ 0.0, 0.0, 0.0 });
	
	const double modVelocidadeInf = Modulus(velInf);
	
	const double mach = modVelocidadeInf / velSomAr;
	const double reynoldsFoguete = (densidadeAr * cRD.cPFogueteData->cAlturaFoguete * modVelocidadeInf) / viscAr;

	double alpha = 0.0;
	if (modVelocidadeInf != 0.0) // Se a velocidade relativa é 0, alpha é zero.
	{
		alpha = AngleBetweenNormalized(ati / Modulus(ati), velInf / modVelocidadeInf);
	}
	
	cPSimulacao->cAlphas.push_back(alpha);
	
	double cdAtriFog = 0.0;
	double cdPressFog = 0.0;
	double cmAlphFog = 0.0;
	double cnAlphFog = 0.0;
	double cmDampFog = 0.0;

	double centroMassaVFog = 0.0;
	double posVOffsetPart = 0.0;
		
	for (size_t i = 0; i < cRD.cPFogueteData->cPartesCorpo.size(); i++)
	{
		double cdAtri = cRD.cPFogueteData->cPartesCorpo[i]->PegarCdAtrit(reynoldsFoguete, mach, cRD.cPFogueteData->cAlturaFoguete, cRD.cPFogueteData->cFinenessRatio, cRD.AREAREF);
		cdAtriFog += cdAtri;

		double cdPress = cRD.cPFogueteData->cPartesCorpo[i]->PegarCdPress(mach, cRD.AREAREF);
		cdPressFog += cdPress;

		if (cSaiuHaste)
		{
			double cmAlph = cRD.cPFogueteData->cPartesCorpo[i]->PegarCMAlpha(alpha, cRD.AREAREF, cRD.COMPREF);
			cmAlphFog += cmAlph;

			double cnAlph = cRD.cPFogueteData->cPartesCorpo[i]->PegarCNAlpha(alpha, cRD.AREAREF);
			cnAlphFog += cnAlph;

			double cDamp = cRD.cPFogueteData->cPartesCorpo[i]->PegarCDampin(velInf, omega, cRD.AREAREF, cRD.COMPREF);
			cmDampFog += cDamp;
		}			

		centroMassaVFog += cRD.cPFogueteData->cPartesCorpo[i]->PegarMassa() * (cRD.cPFogueteData->cPartesCorpo[i]->PegarPosVCM() + posVOffsetPart);

		posVOffsetPart += cRD.cPFogueteData->cPartesCorpo[i]->PegarAltura();
	}

	for (size_t i = 0; i < cRD.cPFogueteData->cAletas.size(); i++)
	{
		double cdAtrit = cRD.cPFogueteData->cAletas[i]->PegarCdAtrit(reynoldsFoguete, mach, cRD.cPFogueteData->cAlturaFoguete, cRD.cPFogueteData->cFinenessRatio, cRD.AREAREF);
		cdAtriFog += cdAtrit;

		double cdPress = cRD.cPFogueteData->cAletas[i]->PegarCdPress(mach, cRD.AREAREF);
		cdPressFog += cdPress;
		
		if (cSaiuHaste)
		{
			double cmAlph = cRD.cPFogueteData->cAletas[i]->PegarCMAlpha(alpha, cRD.AREAREF, cRD.COMPREF);
			cmAlphFog += cmAlph;

			double cnAlph = cRD.cPFogueteData->cAletas[i]->PegarCNAlpha(mach,
				cRD.cPFogueteData->cAletas[i]->PegarRaioNaAleta(), cRD.AREAREF);
			cnAlphFog += cnAlph;

			double cdamp = cRD.cPFogueteData->cAletas[i]->PegarCDampin(velInf, omega,
				cRD.cPFogueteData->cPosVCGFoguete - (cRD.cPFogueteData->cAlturaFoguete + cRD.cPFogueteData->cAletas[i]->PegarPosVCp()),
				cRD.AREAREF, cRD.COMPREF);
			cmDampFog += cdamp;
		}

		centroMassaVFog += 
			(cRD.cPFogueteData->cAletas[i]->PegarPosV() - cRD.cPFogueteData->cAletas[i]->PegarCordaRaiz() + 
				cRD.cPFogueteData->cAletas[i]->PegarPosVCM()) * cRD.cPFogueteData->cAletas[i]->PegarMassa();
	}	
	
	centroMassaVFog += cRD.cPFogueteData->cPMotor->PegMassa(tempoAgora) * 
		(cRD.cPFogueteData->cPMotor->PegarPosVertical() - cRD.cPFogueteData->cPMotor->PegComprimento() + cRD.cPFogueteData->cPMotor->PegarPosVCM());

	for (size_t i = 0; i < cRD.cPFogueteData->cMassasPontos.size(); i++)
	{
		centroMassaVFog += cRD.cPFogueteData->cMassasPontos[i]->PegarMassa() * cRD.cPFogueteData->cMassasPontos[i]->PegarPosVert();
	}

	centroMassaVFog /= massaAgora;

	const double momentoInerciaFogueteX = CalcMomentoInerciaX(centroMassaVFog, tempoAgora);

	double cdPressBase = cRD.cPFogueteData->cPartesCorpo.back()->PegarCdPressBase(mach, cRD.AREAREF);

	VETOR vetorPivo = CrossProduct(ati, vel);
	const double modVetorPivo = Modulus(vetorPivo);

	VETOR dirForcaNormal = {};

	if (modVetorPivo != 0.0) // se o produto vetorial entre ati e velocidade é diferente de zero(não paralelo)(existe angulo entre eles)
	{
		vetorPivo /= modVetorPivo;		

		dirForcaNormal = CrossProduct(ati, vetorPivo);

		dirForcaNormal /= Modulus(dirForcaNormal);
	}

	double cdTotalFoguete = cdAtriFog + cdPressFog + cdPressBase;
		
	double cForcaNormal = (cnAlphFog * alpha) * cRD.AREAREF * 0.5 * densidadeAr * modVelocidadeInf * modVelocidadeInf;

	double cForcasArrasto = cdTotalFoguete * cRD.AREAREF * 0.5 * densidadeAr * modVelocidadeInf * modVelocidadeInf;

	double momento = -cmAlphFog * 0.5 * cRD.AREAREF * cRD.COMPREF * densidadeAr * modVelocidadeInf * modVelocidadeInf
		/ momentoInerciaFogueteX;

	double momentoDamping = cmDampFog * 0.5 * cRD.AREAREF * cRD.COMPREF * densidadeAr * modVelocidadeInf * modVelocidadeInf
		/ momentoInerciaFogueteX;

	momento -= momentoDamping;

	double cNTotal = cnAlphFog;

	omega += vetorPivo * cPSimulacao->cDeltaTime * momento;
	
	MATRIXTRANS mRotPiv;
	double deltaTheta = cPSimulacao->cDeltaTime * Modulus(omega);

	if (deltaTheta != 0.0)
		mRotPiv.RotationFromAxis(deltaTheta, omega);
	else
		mRotPiv.Identity();


	cPSimulacao->cCds.push_back(cdTotalFoguete);
	cPSimulacao->cCMs.push_back(cmAlphFog);
	cPSimulacao->cCNs.push_back(cNTotal);
	cPSimulacao->cMachs.push_back(mach);
	
	VETOR velInfUnit = { velInf.x, velInf.y, velInf.z };

	if (modVelocidadeInf != 0.0)
	{
		velInfUnit /= modVelocidadeInf;
	}
	else
	{

		velInfUnit = { 0.0, 0.0, 0.0 };
	}

	ati *= mRotPiv;
	ati /= Modulus(ati);
	
	VETOR cAccArrasto = { -velInfUnit.x, -velInfUnit.y, -velInfUnit.z };
	cAccArrasto *= cForcasArrasto / massaAgora;

	VETOR accForNormal = dirForcaNormal * (cForcaNormal / massaAgora);

	VETOR g = { 0.0, -9.78, 0.0 };

	acel = {};
	acel += g;
	acel += cAccArrasto;
	acel += accForNormal;
	acel += ati *(cRD.cPFogueteData->cPMotor->PegEmpuxo(tempoAgora) / massaAgora);

	vel += acel * cPSimulacao->cDeltaTime;

	pos += vel * cPSimulacao->cDeltaTime;
	
	cPSimulacao->cAceleracoes.push_back(acel);
	cPSimulacao->cVelocidades.push_back(vel);
	cPSimulacao->cPosicoes.push_back(pos);
	cPSimulacao->cAtitudes.push_back(ati);
	cPSimulacao->cVelocidadesAngulares.push_back(omega);
	cPSimulacao->cAceleracoesAngulares.push_back(vetorPivo * momento);
}

void RocketSim::TickParaquedas(double tempoAgora)
{
	VETOR vel = cPSimulacao->cVelocidades.back();
	VETOR acel = cPSimulacao->cAceleracoes.back();
	VETOR pos = cPSimulacao->cPosicoes.back();
	VETOR ati = cPSimulacao->cAtitudes.back();
	VETOR omega = cPSimulacao->cVelocidadesAngulares.back();

	cPSimulacao->cAlphas.push_back(0.0);
	
	cIntStdAtmosphere.SetAltitude(pos.y + cRD.cAltitudeLocal, cRD.cTemperaturaLocal);
	const double densidadeAr = cIntStdAtmosphere.GetAirDensity();
	const double velSomAr = cIntStdAtmosphere.GetSoundVelocity();

	const double massaAgora = cRD.cPFogueteData->PegarMassaFoguete(tempoAgora);

	VETOR velInf = vel - VETOR({ 0.0, 0.0, 0.0 });

	const double modVelocidadeInf = Modulus(velInf);

	const double mach = modVelocidadeInf / velSomAr;
	
	double cdTotalFoguete = cRD.cPFogueteData->cPParaquedas->PegarCd(cRD.AREAREF);
		
	double cForcasArrasto = cdTotalFoguete * cRD.AREAREF * 0.5 * densidadeAr * modVelocidadeInf * modVelocidadeInf;
		
	cPSimulacao->cCds.push_back(cdTotalFoguete);
	cPSimulacao->cCMs.push_back(-0.0);
	cPSimulacao->cCNs.push_back(-0.0);
	cPSimulacao->cMachs.push_back(mach);

	VETOR velInfUnit = { velInf.x, velInf.y, velInf.z };

	if (modVelocidadeInf != 0.0)
	{
		velInfUnit /= modVelocidadeInf;
	}
	else
	{

		velInfUnit = { 0.0, 0.0, 0.0 };
	}

	VETOR cAccArrasto = { -velInfUnit.x, -velInfUnit.y, -velInfUnit.z };
	cAccArrasto *= cForcasArrasto / massaAgora;

	VETOR g = { 0.0, -9.78, 0.0 };
	
	acel = {};
	acel += g;
	acel += cAccArrasto;
	acel += ati *(cRD.cPFogueteData->cPMotor->PegEmpuxo(tempoAgora) / massaAgora);

	vel += acel * cPSimulacao->cDeltaTime;

	pos += vel * cPSimulacao->cDeltaTime;

	ati = {0.0, 1.0, 0.0};

	cPSimulacao->cAceleracoes.push_back(acel);
	cPSimulacao->cVelocidades.push_back(vel);
	cPSimulacao->cPosicoes.push_back(pos);
	cPSimulacao->cAtitudes.push_back(ati);
	cPSimulacao->cVelocidadesAngulares.push_back({0.0, 0.0, 0.0});
	cPSimulacao->cAceleracoesAngulares.push_back({ 0.0, 0.0, 0.0 });
}

void RocketSim::SimularAteChao(double deltaTime)
{
	LimparAntigaSimulacao();
	
	cPSimulacao = new RocketSimData();
	cPSimulacao->cDeltaTime = deltaTime;

	cPSimulacao->cAceleracoes.push_back(cRD.cAcelInicial);

	cPSimulacao->cVelocidades.push_back(cRD.cVelInicial);

	cPSimulacao->cPosicoes.push_back(cRD.cPosInicial);

	cPSimulacao->cAtitudes.push_back(cRD.cAtiInicial);

	cPSimulacao->cCds.push_back(0.0);

	cPSimulacao->cCNs.push_back(0.0);

	cPSimulacao->cCMs.push_back(0.0);

	cPSimulacao->cMachs.push_back(0.0);

	cPSimulacao->cVelocidadesAngulares.push_back({});

	cPSimulacao->cAceleracoesAngulares.push_back({});

	cPSimulacao->cAlphas.push_back(0.0);

	double tempoSimulacao = 0.0;
	
	const double delayParaquedas = cRD.cPFogueteData->cPParaquedas->PegarDelay();

	while (tempoSimulacao < delayParaquedas && cPSimulacao->cPosicoes.back().y >= 0.0)
	{
		TickDefault(tempoSimulacao);
		tempoSimulacao += deltaTime;
	}

	while (cPSimulacao->cPosicoes.back().y >= 0.0)
	{
		TickParaquedas(tempoSimulacao);
		tempoSimulacao += deltaTime;
	}

	cPSimulacao->cRodouSimulacao = true;
	cPSimulacao->cTempoTotal = tempoSimulacao;

	double altMax = 0.0;
	size_t idMax = 0;
	for (size_t i = 0; i < cPSimulacao->cPosicoes.size(); i++)
	{
		if (altMax < cPSimulacao->cPosicoes[i].y)
		{
			altMax = cPSimulacao->cPosicoes[i].y;
			idMax = i;
		}
	}

	cPSimulacao->cAltitudeMax = altMax;

	double velMax = 0.0;
	for (size_t i = 0; i < cPSimulacao->cVelocidades.size(); i++)
	{
		const double vel = ModulusSquared(cPSimulacao->cVelocidades[i]);
		if (velMax < vel)
		{
			velMax = vel;
		}
	}

	cPSimulacao->cVelocidadeMax = sqrt(velMax);

	double acelMax = 0.0;
	size_t iAcelMax = 0;
	for (size_t i = 0; i < cPSimulacao->cAceleracoes.size(); i++)
	{
		const double acel = ModulusSquared(cPSimulacao->cAceleracoes[i]);
		if (acelMax < acel)
		{
			acelMax = acel;
			iAcelMax = i;
		}
	}

	cPSimulacao->cAceleracaoMax = sqrt(acelMax);

	double alphaMax = 0.0;
	int alphaMaxI = 0;
	for (size_t i = 0; i < cPSimulacao->cAlphas.size(); i++)
	{
		const double alpha = cPSimulacao->cAlphas[i];

		if (abs(alphaMax) < abs(alpha))
		{
			alphaMax = alpha;
			alphaMaxI = i;
		}
	}

	cPSimulacao->cAlphaMaximo = alphaMax;
}

void RocketSim::SimularSubida(double deltaTime)
{
	LimparAntigaSimulacao();

	cPSimulacao = new RocketSimData();
	cPSimulacao->cDeltaTime = deltaTime;

	cPSimulacao->cAceleracoes.push_back(cRD.cAcelInicial);

	cPSimulacao->cVelocidades.push_back(cRD.cVelInicial);

	cPSimulacao->cPosicoes.push_back(cRD.cPosInicial);

	cPSimulacao->cAtitudes.push_back(cRD.cAtiInicial);

	cPSimulacao->cCds.push_back(0.0);

	cPSimulacao->cCNs.push_back(0.0);

	cPSimulacao->cCMs.push_back(0.0);

	cPSimulacao->cMachs.push_back(0.0);

	cPSimulacao->cAlphas.push_back(0.0);

	cPSimulacao->cVelocidadesAngulares.push_back({});

	cPSimulacao->cAceleracoesAngulares.push_back({});

	double tempoSimulacao = 0.0;

	const double delayParaquedas = cRD.cPFogueteData->cPParaquedas->PegarDelay();

	while ((tempoSimulacao < delayParaquedas && cPSimulacao->cVelocidades.back().y > 0.0) || !cSaiuHaste)
	{
		TickDefault(tempoSimulacao);
		tempoSimulacao += deltaTime;
	}

	while (cPSimulacao->cVelocidades.back().y > 0.0)
	{
		TickParaquedas(tempoSimulacao);
		tempoSimulacao += deltaTime;
	}

	cPSimulacao->cRodouSimulacao = true;
	cPSimulacao->cTempoTotal = tempoSimulacao;

	double altMax = 0.0;
	size_t idMax = 0;
	for (size_t i = 0; i < cPSimulacao->cPosicoes.size(); i++)
	{
		if (altMax < cPSimulacao->cPosicoes[i].y)
		{
			altMax = cPSimulacao->cPosicoes[i].y;
			idMax = i;
		}
	}

	cPSimulacao->cAltitudeMax = altMax;

	double velMax = 0.0;
	for (size_t i = 0; i < cPSimulacao->cVelocidades.size(); i++)
	{
		const double vel = ModulusSquared(cPSimulacao->cVelocidades[i]);
		if (velMax < vel)
		{
			velMax = vel;
		}
	}

	cPSimulacao->cVelocidadeMax = sqrt(velMax);

	double acelMax = 0.0;
	size_t iAcelMax = 0;
	for (size_t i = 0; i < cPSimulacao->cAceleracoes.size(); i++)
	{
		const double acel = ModulusSquared(cPSimulacao->cAceleracoes[i]);
		if (acelMax < acel)
		{
			acelMax = acel;
			iAcelMax = i;
		}
	}

	cPSimulacao->cAceleracaoMax = sqrt(acelMax);

	double alphaMax = 0.0;
	int alphaMaxI = 0;
	for (size_t i = 0; i < cPSimulacao->cAlphas.size(); i++)
	{
		const double alpha = cPSimulacao->cAlphas[i];

		if (abs(alphaMax) < abs(alpha))
		{
			alphaMax = alpha;
			alphaMaxI = i;
		}
	}

	cPSimulacao->cAlphaMaximo = alphaMax;
}

double RocketSim::CalcMomentoInerciaX(double cm, double tempoAtual) const
{
	double alt = 0.0;
	double momentoIn = 0.0;
	for (size_t i = 0; i < cRD.cPFogueteData->cPartesCorpo.size(); i++)
	{
		double distCMTopo = abs(cm - (cRD.cPFogueteData->cPartesCorpo[i]->PegarPosVCM() + alt));

		const double momentoParte = cRD.cPFogueteData->cPartesCorpo[i]->PegarMomentoInerciaX() +
			(cRD.cPFogueteData->cPartesCorpo[i]->PegarMassa() * pow(distCMTopo, 2));

		momentoIn += momentoParte;

		alt += cRD.cPFogueteData->cPartesCorpo[i]->PegarAltura();
	}

	for (size_t i = 0; i < cRD.cPFogueteData->cAletas.size(); i++)
	{
		double distCMTopo = cm - 
			(-cRD.cPFogueteData->cAletas[i]->PegarPosVCM() + cRD.cPFogueteData->cAletas[i]->PegarPosV());

		const double momentoAleta = cRD.cPFogueteData->cAletas[i]->PegarMomentoInerciaX() +
			(cRD.cPFogueteData->cAletas[i]->PegarMassa() * pow(distCMTopo, 2));

		momentoIn += momentoAleta;
	}

	for (size_t i = 0; i < cRD.cPFogueteData->cMassasPontos.size(); i++)
	{
		double distCMTopo = cm - cRD.cPFogueteData->cMassasPontos[i]->PegarPosVert();

		const double momentoPonto = cRD.cPFogueteData->cAletas[i]->PegarMassa() * pow(distCMTopo, 2);

		momentoIn += momentoPonto;
	}

	const double momentoMotor = cRD.cPFogueteData->cPMotor->PegarMomentoInerciaX(tempoAtual) +
		(cRD.cPFogueteData->cPMotor->PegMassa(tempoAtual) *
			pow(cm - (cRD.cPFogueteData->cPMotor->PegarPosVertical() - cRD.cPFogueteData->cPMotor->PegarPosVCM()), 2));

	momentoIn += momentoMotor;

	const double momentoParaquedas = 
		cRD.cPFogueteData->cPParaquedas->PegarMassa() * pow(cm - cRD.cPFogueteData->cPParaquedas->PegarPosVert(), 2);

	momentoIn += momentoParaquedas;

	return momentoIn;
}

RocketSim::InternationalStandardAtmosphere::InternationalStandardAtmosphere()
{}

RocketSim::InternationalStandardAtmosphere::~InternationalStandardAtmosphere()
{}

void RocketSim::InternationalStandardAtmosphere::SetAltitude(double altitude, double temperaturaLocalCelsius)
{
	const double tempK = (temperaturaLocalCelsius - (altitude * 6.5 / 1000.0)) + 273.15;
	const double dPdY = (22632.0 - 101325.0) / 11000.0;
	const double pressure = 101325.0 + (dPdY * altitude);

	const double R = 8.31;
	const double M = 28.97e-3;

	// RHO = M * pressure / (R * temp)

	cDensity = (M * pressure) / (R * tempK);

	cSoundVelocity = 331.3 * sqrt(1.0 + ((tempK - 273.15) / (273.15)));

	cViscosity = 1.5*10e-5;
}

double RocketSim::InternationalStandardAtmosphere::GetAirDensity() const
{
	return cDensity;
}

double RocketSim::InternationalStandardAtmosphere::GetSoundVelocity() const
{
	return cSoundVelocity;
}

double RocketSim::InternationalStandardAtmosphere::GetAirViscosity() const
{
	return cViscosity;
}