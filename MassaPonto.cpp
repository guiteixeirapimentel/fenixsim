#include "MassaPonto.h"

MassaPonto::MassaPonto(double massa, double posV)
	:
	cMassa(massa),
	cPosVertical(posV)
{}

MassaPonto::~MassaPonto()
{}

double MassaPonto::PegarMassa() const
{
	return cMassa;
}

double MassaPonto::PegarPosVert() const
{
	return cPosVertical;
}