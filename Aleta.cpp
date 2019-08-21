#include "Aleta.h"

Aleta::Aleta(double densidade, double rugosidade, int numeroAletas, double envergadura, double espessura, PERFIL perfil, double posVertical, double raioNaAleta)
	:
	cDensidade(densidade),
	cRugosidade(rugosidade),
	cEnvergadura(envergadura),
	cEspessura(espessura),
	cNumeroAletas(numeroAletas),
	cPerfil(perfil),
	cPosV(posVertical),
	cRaioNaAleta(raioNaAleta)
{}

Aleta::~Aleta()
{}

double Aleta::PegarPosVCp() const
{
	return cPosVCP;
}

double Aleta::PegarPosVCM() const
{
	return cPosVCM;
}

double Aleta::PegarVolume() const
{
	return cVolume;
}

double Aleta::PegarMassa() const
{
	return cMassa;
}

double Aleta::PegarPosV() const
{
	return cPosV;
}

double Aleta::PegarEnvergadura() const
{
	return cEnvergadura;
}
double Aleta::PegarEspessura() const
{
	return cEspessura;
}
	
double Aleta::PegarCordaRaiz() const
{
	return 0.0;
}
double Aleta::PegarCordaPonta() const
{
	return 0.0;
}
double Aleta::PegarDeslCordaPonta() const
{
	return 0.0;
}

int Aleta::PegarNumAletas() const
{
	return cNumeroAletas;
}

double Aleta::PegarRaioNaAleta() const
{
	return cRaioNaAleta;
}

void Aleta::AlterarRaioNaAleta(double nraio)
{
	cRaioNaAleta = nraio;
}

double Aleta::PegarDensidade() const
{
	return cDensidade;
}

double Aleta::PegarRugosidade() const
{
	return cRugosidade;
}

double Aleta::PegarMomentoInerciaX() const
{
	return cMomentoInerciaX;
}