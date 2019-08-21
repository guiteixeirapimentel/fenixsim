#include "SimetricaAxial.h"

SimetricaAxial::SimetricaAxial(double densidade, double rugosidade, double altura)
	:
	cDensidade(densidade),
	cRugosidade(rugosidade),
	cAltura(altura)
{}

SimetricaAxial::~SimetricaAxial()
{}

double SimetricaAxial::PegarVolumeAparente() const
{
	return cVolumeAparente;
}

double SimetricaAxial::PegarVolumeReal() const
{
	return cVolumeReal;
}

double SimetricaAxial::PegarMassa() const
{
	return cMassa;
}

double SimetricaAxial::PegarPosVCp() const
{
	return cPosVCP;
}

double SimetricaAxial::PegarPosVCM() const
{
	return cPosVCM;
}

double SimetricaAxial::PegarAltura() const
{
	return cAltura;
}
double SimetricaAxial::PegarRaioBaseInt() const 
{
	return 0.0;
}
double SimetricaAxial::PegarRaioTopoInt() const
{
	return 0.0;
}
double SimetricaAxial::PegarRaioBaseExt() const
{
	return 0.0;
}
double SimetricaAxial::PegarRaioTopoExt() const
{
	return 0.0;
}

double SimetricaAxial::PegarDensidade() const
{
	return cDensidade;
}

double SimetricaAxial::PegarRugosidade() const
{
	return cRugosidade;
}

double SimetricaAxial::PegarMomentoInerciaX() const
{
	return cMomentoInerciaX;
}