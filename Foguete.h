#pragma once
#include <vector>
#include <stdint.h>

#include "Aleta.h"
#include "SimetricaAxial.h"
#include "MassaPonto.h"

#include "Motor.h"
#include "Paraquedas.h"

class Foguete
{
public:
	enum TIPOPARTECORPO : int32_t
	{
		TUBO,
		COIFACONICA,
		TRANSICAOAUMENTO,
		TRANSICAODIMINUI
	};
	enum TIPOALETA : int32_t
	{
		TRAPEZOIDAL
	};

public:
	Foguete();
	~Foguete();

	bool Salvar(const std::string& nomeArq) const;
	bool Abrir(const std::string& nomeArq);

	void CriarAletaTrapezoidal(double rugosidade, double espessura, double densidade, int numAleta, Aleta::PERFIL perfil, double cordaRaiz,
		double cordaPonta, double envergadura, double deslCordaPonta, double posVertical);
	void CriarTubo(double rugosidade, double densidade, double raioExt, double raioInt, double altura);
	void CriarTransicao(double rugosidade, double densidade, double raioExtTopo, double raioIntTopo, 
		double raioExtBase, double raioIntBase, double altura);
	void CriarCoifaConica(double rugosidade, double densidade, double raioBaseExt, double raioBaseInt, double altura);
	void CriarMassaPonto(double massa, double posV);	
	void DefinirMotor(const std::string& pathArqMotor, double posVertical);
	void DefinirParaquedas(double delayDeploy, double CD, double diametro, double massa, double posVertical);

	bool MudarAletaTrapezoidal(double rugosidade, double espessura, double densidade, int numAleta, Aleta::PERFIL perfil, double cordaRaiz,
		double cordaPonta, double envergadura, double deslCordaPonta, double posVertical, size_t id);
	bool MudarTubo(double rugosidade, double densidade, double raioExt, double raioInt, double altura, size_t id);
	bool MudarCoifaConica(double rugosidade, double densidade, double raioBaseExt, double raioBaseInt, double altura, size_t id);
	bool MudarMassaPonto(double massa, double posV, size_t id);
	bool MudarTransicao(double rugosidade, double densidade, double raioExtTopo, double raioIntTopo,
		double raioExtBase, double raioIntBase, double altura, uint32_t id);

	bool DeletarAleta(size_t id);
	bool DeletarParteCorpo(size_t id);

	double PegarMassaFoguete(double tempo) const;

private:
	void CalcularDadosFoguete();

	double PegarRaioTuboNaAleta(double posVertAleta) const;

private:
	class FILEINFO
	{
	public:		
		FILEINFO(uint32_t numPartesCorpo, uint32_t numConjAletas, uint32_t numCharsMotorPath, 
			uint32_t cNumCharsNomeMotor, double posMotor);
		const uint32_t cVer = 0x00000005;
		const uint32_t cNumPartesCorpo;
		const uint32_t cNumConjAletas;
		const uint32_t cNumCharsMotorPath;
		const uint32_t cNumCharsNomeFoguete;
		const double cPosTopoMotor;
	};
public:
	std::string cNomeFoguete;
	const double AREAREF = 28.27e-4;
	const double COMPREF = 0.6;

	std::vector<SimetricaAxial*> cPartesCorpo;
	std::vector<Aleta*> cAletas;
	std::vector<MassaPonto*> cMassasPontos;

	std::vector<TIPOPARTECORPO> cTipoPartes;
	std::vector<TIPOALETA> cTipoAletas;

	Motor* cPMotor;

	Paraquedas* cPParaquedas;

	double cAlturaFoguete;
	
	double cPosVCPFoguete;
	double cPosVCGFoguete;
	double cMassaFogueteComMotor;

	double cMassaFogueteSemMotor;

	double cRaioMaximo;

	double cFinenessRatio;
};