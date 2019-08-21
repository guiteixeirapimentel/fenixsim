#include "DataSimHandler.h"
#include <fstream>
#include <cassert>

DataSimHandler::DataSimHandler()
{}

DataSimHandler::~DataSimHandler()
{}

void DataSimHandler::SaveCSVPositionInfo(const std::string& filePath, const RocketSimData* pRSD, size_t numPontos) const
{
	if (numPontos != 0)
	{

	}
	else
	{
		std::ofstream arq(filePath);
		if (!arq.is_open())
		{
			//ERRO AO ABRIR ARQUIVO
			assert(0);
			return;
		}

		arq << "Tempo;PosX;PosY;PosZ\n";

		double t = 0.0;

		for(size_t  i = 0; i < pRSD->cPosicoes.size(); i++)
		{
			arq << t << ";" << std::to_string(pRSD->cPosicoes[i].x) << ";" << 
				std::to_string(pRSD->cPosicoes[i].y) << ";" << std::to_string(pRSD->cPosicoes[i].z) << "\n";

			t += pRSD->cDeltaTime;
		}

		arq.close();
	}
}