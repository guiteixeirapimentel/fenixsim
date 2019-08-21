#include "Motor.h"
#include <fstream>
#include <cassert>
#include <string>
#include <sstream>
#include <Windows.h>

std::vector<std::string> explode(std::string const & s, char delim)
{
	std::vector<std::string> result;
	std::istringstream iss(s);

	for (std::string token; std::getline(iss, token, delim); )
	{
		result.push_back(std::move(token));
	}

	return result;
}

Motor::Motor(const std::string& arqCurvaEmpuxo, double posVertical)
	:
	cMotorValido(true),
	cPosVerticalMotor(posVertical),
	cMotorPath(arqCurvaEmpuxo)
{
	std::ifstream arq(arqCurvaEmpuxo);

	if (!arq.is_open())
	{
		cMotorValido = false;
		//assert(cMotorValido);
		return;
	}

	bool posHeader = false;
	while (!arq.eof())
	{
		std::string buf;

		std::getline(arq, buf);

		if (buf.c_str()[0] == ';')
		{
			cComentarios += buf;
		}
		else if(!posHeader)
		{
			std::string t;
			std::vector<std::string> buf2 = explode(buf, ' ');

			if (buf2.size() != 7)
			{
				cMotorValido = false;
				MessageBox(NULL, L"Erro ao obter curva empuxo. Header inválido.", L"Erro", 0);
				break;
			}
			cNomeMotor = buf2[0];
			cDiametro = std::atof(buf2[1].c_str()) / 1000.0;
			cComprimento = std::atof(buf2[2].c_str()) / 1000.0;
			
			std::vector<std::string> bufAtrasosEjecao = explode(buf2[3], '-');

			for (size_t i = 0; i < bufAtrasosEjecao.size(); i++)
			{
				cAtrasosEjecao.push_back(std::atof(bufAtrasosEjecao[i].c_str()));
			}

			cMassaPropelente = std::atof(buf2[4].c_str());
			cMassaTotal = std::atof(buf2[5].c_str());
			cNomeMotor += " - " + buf2[6];

			posHeader = true;
		}
		else
		{
			std::vector<std::string> buf2 = explode(buf, ' ');

			if (buf2.size() != 2)
			{
				cMotorValido = false;
				MessageBox(NULL, L"Erro ao obter curva empuxo. Pontos inválidos. Confira se o arquivo termina com ponto e virgula.", L"Erro", 0);
			}

			Ponto p;
			p.tempo = std::atof(buf2[0].c_str());
			p.empuxo = std::atof(buf2[1].c_str());

			cPontos.push_back(p);
		}
	}

	if(cPontos.size() > 0)
		cTempoQueima = cPontos.back().tempo;
}

Motor::~Motor()
{

}

double Motor::PegarMomentoInerciaX(double tempoAgora) const
{
	return (PegMassa(tempoAgora) / 12.0) * ((3.0 * pow(cDiametro / 2.0, 2)) + pow(cComprimento, 2));
}