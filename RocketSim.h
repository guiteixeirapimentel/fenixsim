#pragma once
#include "LinAlg.h"
#include "FunctionMath.h"
#include <vector>
#include <list>
#include "Motor.h"
#include "Foguete.h"


class RocketSimDesc
{
public:
	RocketSimDesc()
	{}
public:
	const double AREAREF = 28.3e-4;
	const double COMPREF = 0.06;

	VETOR cPosInicial;
	VETOR cVelInicial;
	VETOR cAcelInicial;
	VETOR cAtiInicial;

	double cAltitudeLocal;
	double cTemperaturaLocal;

	double cComprimentoHaste;
	
	const Foguete* cPFogueteData;

	double cReynoldsCriticoCoifa;
	double cReynoldsCriticoCorpo;
	double cReynoldsCriticoAletas;

	Motor* cPMotor;
};

struct RocketSimData
{
	bool cRodouSimulacao;

	double cDeltaTime;
	std::vector<VETOR> cPosicoes;
	std::vector<VETOR> cVelocidades;
	std::vector<VETOR> cAceleracoes;
	std::vector<VETOR> cAtitudes;
	std::vector<VETOR> cAceleracoesAngulares;
	std::vector<VETOR> cVelocidadesAngulares;

	std::vector<double> cCds;
	std::vector<double> cCNs;
	std::vector<double> cCMs;
	std::vector<double> cMachs;
	std::vector<double> cAlphas;
	
	bool cLevantouVoo;

	double cTempoTotal;
	double cVelocidadeMax;
	double cAceleracaoMax;
	double cAltitudeMax;
	double cAlphaMaximo;
};

class RocketSim
{
public:
	class InternationalStandardAtmosphere
	{
	public:
		InternationalStandardAtmosphere();
		~InternationalStandardAtmosphere();

		void SetAltitude(double altitude, double temperaturaLocalCelsius);

		double GetSoundVelocity() const;
		double GetAirDensity() const;
		double GetAirViscosity() const;

	private:
		double cSoundVelocity;
		double cDensity;
		double cViscosity;
	};
public:
	RocketSim(const RocketSimDesc& rocketDesc);
	RocketSim() = delete;
	RocketSim(const RocketSim& rs) = delete;
	~RocketSim();

	void Simular(double maxTime, double deltaTime = 0.01);
	void SimularAteChao(double deltaTime = 0.01);
	void SimularSubida(double deltaTime = 0.01);

	const RocketSimData* PegarDadosSim() const
	{
		return cPSimulacao;
	}

private:
	double CalcMomentoInerciaX(double cm, double tempoAtual) const;

private:
	void LimparAntigaSimulacao()
	{
		if (cPSimulacao)
		{
			delete cPSimulacao;
			cPSimulacao = nullptr;
		}

		cSaiuHaste = false;
	}
	
	void TickDefault(double tempoAgora);
	void TickParaquedas(double tempoAgora);

private:
	RocketSimData* cPSimulacao;

	InternationalStandardAtmosphere cIntStdAtmosphere;

	RocketSimDesc cRD;

	bool cSaiuHaste;
};