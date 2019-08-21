#pragma once
#include <cmath>
#include <vector>
#include <string>

class Motor
{
private:
	struct Ponto
	{
		double tempo;
		double empuxo;
	};
public:
	Motor(const std::string& arqCurvaEmpuxo, double posVertical);
	~Motor();

	inline double PegEmpuxo(double t) const
	{
		if (t < cTempoQueima)
		{
			for (size_t i = 0; i < cPontos.size(); i++)
			{
				if (cPontos[i].tempo > t)
				{
					if (i > 0)
					{
						double res = cPontos[i].empuxo + 
							((cPontos[i - 1].empuxo - cPontos[i].empuxo) / (cPontos[i - 1].tempo - cPontos[i].tempo) * (t - cPontos[i].tempo));
						return res;
					}
					else if (i + 1 < cPontos.size())
					{
						double res = cPontos[i].empuxo +
							((cPontos[i+1].empuxo - cPontos[i].empuxo) / (cPontos[i+1].tempo - cPontos[i].tempo) * (t - cPontos[i].tempo));
						return res;
					}
				}
			}
		}
		return 0.0;
	}
	inline double PegMassa(double t) const
	{
		if (t < cTempoQueima)
		{
			return cMassaTotal - ((t/cTempoQueima) * cMassaPropelente);
		}
		return cMassaTotal - cMassaPropelente;
	}
	double PegDiametro() const
	{
		return cDiametro;
	}
	double PegComprimento() const
	{
		return cComprimento;
	}

	bool Valido() const
	{
		return cMotorValido;
	}
	std::string PegNome() const
	{
		return cNomeMotor;
	}
	double PegTempoQueima() const
	{
		return cTempoQueima;
	}

	double PegarPosVertical() const
	{
		return cPosVerticalMotor;
	}

	double PegarPosVCM() const
	{
		return cComprimento / 2.0;
	}
	void MudarPosVertical(double posV)
	{
		cPosVerticalMotor = posV;
	}

	std::string PegarCaminhoMotor() const
	{
		return cMotorPath;
	}

	double PegarMomentoInerciaX(double tempoAgora) const;

private:
	bool cMotorValido;
	std::vector<Ponto> cPontos;
	double cTempoQueima;
	double cMassaTotal;
	double cMassaPropelente;
	double cDiametro;
	double cComprimento;
	
	std::string cNomeMotor;
	std::vector<double> cAtrasosEjecao;
	std::string cComentarios;

	double cPosVerticalMotor;

	std::string cMotorPath;
};