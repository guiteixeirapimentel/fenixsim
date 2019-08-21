#pragma once

class MassaPonto
{
public:
	MassaPonto(double massa, double posVertical);
	~MassaPonto();

	double PegarMassa() const;
	double PegarPosVert() const;
		
private:
	double cMassa;
	double cPosVertical;
};