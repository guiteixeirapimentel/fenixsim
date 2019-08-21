#include "Foguete.h"

#include <fstream>

#include "Trapezoidal.h"
#include "Conica.h"
#include "Tubo.h"
#include "AumentoRaio.h"
#include "DiminuiRaio.h"
#include "MassaPonto.h"

Foguete::Foguete()
	:
	cPMotor(nullptr),
	cPParaquedas(nullptr)
{}

Foguete::~Foguete()
{
	for (size_t i = 0; i < cPartesCorpo.size(); i++)
	{
		if (cPartesCorpo[i] != nullptr)
		{
			delete cPartesCorpo[i];
			cPartesCorpo[i] = nullptr;
		}
	}
	cPartesCorpo.clear();
	cTipoPartes.clear();

	for (size_t i = 0; i < cAletas.size(); i++)
	{
		if (cAletas[i] != nullptr)
		{
			delete cAletas[i];
			cAletas[i] = nullptr;
		}
	}
	cAletas.clear();
	cTipoAletas.clear();

	for (size_t i = 0; i < cMassasPontos.size(); i++)
	{
		if (cMassasPontos[i] != nullptr)
		{
			delete cMassasPontos[i];
			cMassasPontos[i] = nullptr;
		}
	}
	cMassasPontos.clear();

	if (cPMotor)
	{
		delete cPMotor;
		cPMotor = nullptr;
	}

	if (cPParaquedas)
	{
		delete cPParaquedas;
		cPParaquedas = nullptr;
	}
}

void Foguete::CriarAletaTrapezoidal(double rugosidade, double espessura, double densidade, int numAletas, Aleta::PERFIL perfil,
	double cordaRaiz, double cordaPonta, double envergadura, double deslCordaPonta, double posVertical)
{
	Aleta* pAleta = new Trapezoidal(densidade, rugosidade,numAletas, envergadura, espessura, perfil, posVertical, 
		cordaRaiz, cordaPonta, deslCordaPonta, PegarRaioTuboNaAleta(posVertical - cordaRaiz));

	cAletas.push_back(pAleta);

	pAleta = nullptr;

	cTipoAletas.push_back(TRAPEZOIDAL);

	CalcularDadosFoguete();
}

void Foguete::CriarTubo(double rugosidade, double densidade, double raioExt, double raioInt, double altura)
{
	Tubo* pTubo = new Tubo(densidade, rugosidade, altura, raioInt, raioExt);

	cPartesCorpo.push_back(pTubo);

	pTubo = nullptr;

	cTipoPartes.push_back(TUBO);

	CalcularDadosFoguete();
}

void Foguete::CriarCoifaConica(double rugosidade, double densidade, double raioBaseExt, double raioBaseInt, double altura)
{
	Conica* pConica = new Conica(densidade, rugosidade, altura, raioBaseInt, raioBaseExt);

	cPartesCorpo.push_back(pConica);

	pConica = nullptr;

	cTipoPartes.push_back(COIFACONICA);

	CalcularDadosFoguete();
}

void Foguete::CriarTransicao(double rugosidade, double densidade, double raioExtTopo, double raioIntTopo,
	double raioExtBase, double raioIntBase, double altura)
{
	if (raioExtBase > raioExtTopo)
	{
		AumentoRaio* pAumRaio = new AumentoRaio(densidade, rugosidade, altura, raioIntTopo, raioExtTopo, raioIntBase, raioExtBase);

		cPartesCorpo.push_back(pAumRaio);

		pAumRaio = nullptr;

		cTipoPartes.push_back(Foguete::TRANSICAOAUMENTO);

		
	}
	else
	{
		DiminuiRaio* pDimRaio = new DiminuiRaio(densidade, rugosidade, altura, raioIntTopo, raioExtTopo, raioIntBase, raioExtBase);

		cPartesCorpo.push_back(pDimRaio);

		pDimRaio = nullptr;

		cTipoPartes.push_back(Foguete::TRANSICAODIMINUI);
	}

	CalcularDadosFoguete();
}

void Foguete::CriarMassaPonto(double massa, double posV)
{
	cMassasPontos.push_back(new MassaPonto(massa, posV));
	CalcularDadosFoguete();
}

void Foguete::DefinirMotor(const std::string& pathArqMotor, double posVMotor)
{
	if (cPMotor)
	{
		delete cPMotor;
		cPMotor = nullptr;
	}

	cPMotor = new Motor(pathArqMotor, posVMotor);

	CalcularDadosFoguete();
}

void Foguete::DefinirParaquedas(double delayDeploy, double CD, double diametro, double massa, double posVertical)
{
	if (cPParaquedas)
	{
		delete cPParaquedas;
		cPParaquedas = nullptr;
	}

	cPParaquedas = new Paraquedas(massa, posVertical, delayDeploy, diametro, CD);

	CalcularDadosFoguete();
}

bool Foguete::DeletarAleta(size_t id)
{
	if (id >= cAletas.size())
	{
		throw "Tentando deletar aleta de indice não existente!";
	}

	if (cAletas[id])
	{
		delete cAletas[id];
		
		std::vector<Aleta*> tmpAletas;
		std::vector<TIPOALETA> tmpTiposAletas;

		for (size_t i = 0; i < cAletas.size(); i++)
		{
			if (i != id)
			{
				tmpAletas.push_back(cAletas[i]);
				tmpTiposAletas.push_back(cTipoAletas[i]);
			}
		}
		cAletas = tmpAletas;
		cTipoAletas = tmpTiposAletas;
	}
	CalcularDadosFoguete();
	return true;
}

bool Foguete::DeletarParteCorpo(size_t id)
{
	if (id >= cPartesCorpo.size())
	{
		throw "Tentando deletar parte de corpo de indice não existente!";
	}

	if (cPartesCorpo[id])
	{
		delete cPartesCorpo[id];

		std::vector<SimetricaAxial*> tmpParteCorpo;
		std::vector<TIPOPARTECORPO> tmpTiposParte;

		for (size_t i = 0; i < cPartesCorpo.size(); i++)
		{
			if (i != id)
			{
				tmpParteCorpo.push_back(cPartesCorpo[i]);
				tmpTiposParte.push_back(cTipoPartes[i]);
			}
		}
		cPartesCorpo = tmpParteCorpo;
		cTipoPartes = tmpTiposParte;
	}
	CalcularDadosFoguete();
	return true;
}

bool Foguete::MudarAletaTrapezoidal(double rugosidade, double espessura, double densidade, int numAleta, Aleta::PERFIL perfil, double cordaRaiz,
	double cordaPonta, double envergadura, double deslCordaPonta, double posVertical, size_t id)
{
	if (id >= cAletas.size() || cAletas[id] == nullptr)
	{
		throw "Tentando alterar aleta de indice não existente!";
	}
	
	if (cAletas[id])
	{
		delete cAletas[id];

		cAletas[id] = new Trapezoidal(densidade, rugosidade, numAleta, envergadura, espessura, perfil, 
			posVertical, cordaRaiz, cordaPonta, deslCordaPonta, PegarRaioTuboNaAleta(posVertical - cordaRaiz));

		cTipoAletas[id] = TRAPEZOIDAL;
	}

	CalcularDadosFoguete();

	return true;
}

bool Foguete::MudarTubo(double rugosidade, double densidade, double raioExt, double raioInt, double altura, size_t id)
{
	if (id >= cPartesCorpo.size() || cPartesCorpo[id] == nullptr)
	{
		throw "Tentando alterar tubo de indice não existente!";
	}

	if (cPartesCorpo[id])
	{
		delete cPartesCorpo[id];

		cPartesCorpo[id] = new Tubo(densidade, rugosidade, altura, raioInt, raioExt);

		cTipoPartes[id] = TUBO;
	}

	CalcularDadosFoguete();

	return true;
}

bool Foguete::MudarTransicao(double rugosidade, double densidade, double raioExtTopo, double raioIntTopo,
	double raioExtBase, double raioIntBase, double altura, uint32_t id)
{
	if (id >= cPartesCorpo.size() || cPartesCorpo[id] == nullptr)
	{
		throw "Tentando alterar tubo de indice não existente!";
	}

	if (raioExtBase > raioExtTopo)
	{
		AumentoRaio* pAumRaio = new AumentoRaio(densidade, rugosidade, altura, raioIntTopo, raioExtTopo, raioIntBase, raioExtBase);

		delete cPartesCorpo[id];

		cPartesCorpo[id] = pAumRaio;

		pAumRaio = nullptr;

		cTipoPartes[id] = Foguete::TRANSICAOAUMENTO;
	}
	else
	{
		DiminuiRaio* pDimRaio = new DiminuiRaio(densidade, rugosidade, altura, raioIntTopo, raioExtTopo, raioIntBase, raioExtBase);
		
		delete cPartesCorpo[id];

		cPartesCorpo[id] = pDimRaio;

		pDimRaio = nullptr;

		cTipoPartes[id] = Foguete::TRANSICAODIMINUI;
	}

	CalcularDadosFoguete();

	return true;
}

bool Foguete::MudarCoifaConica(double rugosidade, double densidade, double raioBaseExt, double raioBaseInt, double altura, size_t id)
{
	if (id >= cPartesCorpo.size() || cPartesCorpo[id] == nullptr)
	{
		throw "Tentando alterar coifaconica de indice não existente!";
	}

	if (cPartesCorpo[id])
	{
		delete cPartesCorpo[id];

		cPartesCorpo[id] = new Conica(densidade, rugosidade, altura, raioBaseInt, raioBaseExt);

		cTipoPartes[id] = COIFACONICA;
	}

	CalcularDadosFoguete();

	return true;
}

bool Foguete::MudarMassaPonto(double massa, double posV, size_t id)
{
	if (id >= cMassasPontos.size() || cMassasPontos[id] == nullptr)
	{
		throw "Tentando alterar massas pontos de indice não existente!";
	}

	if (cMassasPontos[id])
	{
		delete cMassasPontos[id];

		cMassasPontos[id] = new MassaPonto(massa, posV);
	}

	CalcularDadosFoguete();

	return true;
}

void Foguete::CalcularDadosFoguete()
{
	cAlturaFoguete = 0.0;	
	cMassaFogueteComMotor = 0.0;

	double xcpFoguete = 0.0;
	double denCP = 0.0;

	double xcgFoguete = 0.0;
	
	cRaioMaximo = 0.0;

	for (size_t i = 0; i < cPartesCorpo.size(); i++)
	{
		xcpFoguete += (cPartesCorpo[i]->PegarPosVCp() + cAlturaFoguete) * cPartesCorpo[i]->PegarCNAlpha(0.0, AREAREF);
		denCP += cPartesCorpo[i]->PegarCNAlpha(0.0, AREAREF);

		cRaioMaximo = std::fmax(std::fmax(cRaioMaximo, cPartesCorpo[i]->PegarRaioBaseExt()), cPartesCorpo[i]->PegarRaioTopoExt());

		xcgFoguete += (cPartesCorpo[i]->PegarPosVCM() + cAlturaFoguete) * cPartesCorpo[i]->PegarMassa();

		cMassaFogueteComMotor += cPartesCorpo[i]->PegarMassa();

		cAlturaFoguete += cPartesCorpo[i]->PegarAltura();		
	}
	
	for (size_t i = 0; i < cAletas.size(); i++)
	{
		cAletas[i]->AlterarRaioNaAleta(PegarRaioTuboNaAleta(cAletas[i]->PegarPosV() - cAletas[i]->PegarCordaRaiz()));

		xcpFoguete += (cAletas[i]->PegarPosVCp() + cAletas[i]->PegarPosV() - cAletas[i]->PegarCordaRaiz()) * cAletas[i]->PegarCNAlpha(0.0, cAletas[i]->PegarRaioNaAleta(), AREAREF);
		denCP += cAletas[i]->PegarCNAlpha(0.0, cAletas[i]->PegarRaioNaAleta(), AREAREF);

		xcgFoguete += (-cAletas[i]->PegarPosVCM() + cAletas[i]->PegarPosV()) * cAletas[i]->PegarMassa();

		cMassaFogueteComMotor += cAletas[i]->PegarMassa();
	}

	for (size_t i = 0; i < cMassasPontos.size(); i++)
	{
		xcgFoguete += cMassasPontos[i]->PegarPosVert() * cMassasPontos[i]->PegarMassa();

		cMassaFogueteComMotor += cMassasPontos[i]->PegarMassa();
	}

	if (cPParaquedas)
	{
		xcgFoguete += cPParaquedas->PegarMassa() * cPParaquedas->PegarPosVert();
		cMassaFogueteComMotor += cPParaquedas->PegarMassa();
	}

	cMassaFogueteSemMotor = cMassaFogueteComMotor;

	if (cPMotor)
	{
		xcgFoguete += cPMotor->PegMassa(0.0) * (cPMotor->PegarPosVertical() - (0.5 * cPMotor->PegComprimento()));
		cMassaFogueteComMotor += cPMotor->PegMassa(0.0);

		cMassaFogueteSemMotor -= cPMotor->PegMassa(0.0);
	}

	xcpFoguete /= denCP;
	xcgFoguete /= cMassaFogueteComMotor;	

	cPosVCPFoguete = xcpFoguete;
	cPosVCGFoguete = xcgFoguete;

	cFinenessRatio = cAlturaFoguete / (cRaioMaximo * 2.0);
}

double Foguete::PegarRaioTuboNaAleta(double posVertAleta) const
{		
	double alturaTotal = 0.0;
	for (size_t i = 0; i < cPartesCorpo.size(); i++)
	{
		alturaTotal += cPartesCorpo[i]->PegarAltura();

		if (alturaTotal >= posVertAleta)
		{
			return cPartesCorpo[i]->PegarRaioTopoExt();
		}
	}

	return 0.0;
}

bool Foguete::Salvar(const std::string& nomeArq) const
{
	//ARQUIVO FOGUETE(.FOG):
	// class FILEINFO
	//....
	// TIPOS PARTE CORPO
	// ....
	// TIPOS ALETA
	// ....
	// PARTES CORPO
	// ....
	// ALETAS
	// ....
	// MOTOR FILE PATH
	// ....
	// PARAQUEDAS
	// ....


	std::ofstream arq(nomeArq.c_str(), std::ios::binary);

	if (!arq.is_open())
	{
		return false;
	}

	FILEINFO FI(cPartesCorpo.size(), cAletas.size(), cPMotor->PegarCaminhoMotor().size(), cNomeFoguete.size(), 
		cPMotor->PegarPosVertical());

	arq.write(reinterpret_cast<const char*>(&FI), sizeof(FILEINFO));

	arq.write(reinterpret_cast<const char*>(cTipoPartes.data()), sizeof(TIPOPARTECORPO) * cTipoPartes.size());

	arq.write(reinterpret_cast<const char*>(cTipoAletas.data()), sizeof(TIPOALETA) * cTipoAletas.size());

	for (size_t i = 0; i < cPartesCorpo.size(); i++)
	{
		switch (cTipoPartes[i])
		{
		case COIFACONICA:
			arq.write(reinterpret_cast<char*>(cPartesCorpo[i]), sizeof(Conica));
			break;
		case TUBO:
			arq.write(reinterpret_cast<char*>(cPartesCorpo[i]), sizeof(Tubo));
			break;
		case TRANSICAOAUMENTO:
			arq.write(reinterpret_cast<char*>(cPartesCorpo[i]), sizeof(AumentoRaio));
			break;
		case TRANSICAODIMINUI:
			arq.write(reinterpret_cast<char*>(cPartesCorpo[i]), sizeof(DiminuiRaio));
			break;
		}
	}

	for (size_t i = 0; i < cAletas.size(); i++)
	{
		switch (cTipoAletas[i])
		{
		case TRAPEZOIDAL:
			arq.write(reinterpret_cast<char*>(cAletas[i]), sizeof(Trapezoidal));
			break;
		}
	}

	arq.write(cPMotor->PegarCaminhoMotor().c_str(), cPMotor->PegarCaminhoMotor().size());
	
	arq.write(cNomeFoguete.c_str(), cNomeFoguete.size());

	arq.write(reinterpret_cast<char*>(cPParaquedas), sizeof(Paraquedas));
		
	arq.close();

	return true;
}

bool Foguete::Abrir(const std::string& nomeArq)
{
	for (size_t i = 0; i < cPartesCorpo.size(); i++)
	{
		if (cPartesCorpo[i] != nullptr)
		{
			delete cPartesCorpo[i];
			cPartesCorpo[i] = nullptr;
		}
	}
	cPartesCorpo.clear();
	cTipoPartes.clear();

	for (size_t i = 0; i < cAletas.size(); i++)
	{
		if (cAletas[i] != nullptr)
		{
			delete cAletas[i];
			cAletas[i] = nullptr;
		}
	}
	cAletas.clear();
	cTipoAletas.clear();

	for (size_t i = 0; i < cMassasPontos.size(); i++)
	{
		if (cMassasPontos[i] != nullptr)
		{
			delete cMassasPontos[i];
			cMassasPontos[i] = nullptr;
		}
	}
	cMassasPontos.clear();

	if (cPMotor)
	{
		delete cPMotor;
		cPMotor = nullptr;
	}

	if (cPParaquedas)
	{
		delete cPParaquedas;
		cPParaquedas = nullptr;
	}

	//ARQUIVO FOGUETE(.FOG):
	// class FILEINFO
	//....
	// TIPOS PARTE CORPO
	// ....
	// TIPOS ALETA
	// ....
	// PARTES CORPO
	// ....
	// ALETAS
	// ....
	// MOTOR FILE PATH
	// ....
	// PARAQUEDAS
	// ....


	std::ifstream arq(nomeArq.c_str(), std::ios::binary);

	if (!arq.is_open())
	{
		return false;
	}

	FILEINFO FI(0, 0, 0, 0,  0.0);

	uint32_t ver = FI.cVer;

	arq.read(reinterpret_cast<char*>(&FI), sizeof(FILEINFO));

	if (ver != FI.cVer)
	{
		throw "Versão diferente!";
	}
	
	cTipoPartes.resize(FI.cNumPartesCorpo);
	cTipoAletas.resize(FI.cNumConjAletas);
	
	arq.read(reinterpret_cast<char*>(cTipoPartes.data()), FI.cNumPartesCorpo * sizeof(TIPOPARTECORPO));
	arq.read(reinterpret_cast<char*>(cTipoAletas.data()), FI.cNumConjAletas * sizeof(TIPOALETA));
	
	for (size_t i = 0; i < FI.cNumPartesCorpo; i++)
	{
		switch (cTipoPartes[i])
		{
		case COIFACONICA:
		{
			Conica con(0.0, 0.0, 0.0, 0.0, 0.0);
			arq.read(reinterpret_cast<char*>(&con), sizeof(Conica));
			Conica* pCon = new Conica(con);
			cPartesCorpo.push_back(pCon);
			pCon = nullptr;
		}			
			break;
		case TUBO:
		{
			Tubo t(0.0, 0.0, 0.0, 0.0, 0.0);
			arq.read(reinterpret_cast<char*>(&t), sizeof(Tubo));
			Tubo* pTub = new Tubo(t);
			cPartesCorpo.push_back(pTub);
			pTub = nullptr;
		}
			break;
		case TRANSICAOAUMENTO:
		{
			AumentoRaio t(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			arq.read(reinterpret_cast<char*>(&t), sizeof(AumentoRaio));
			AumentoRaio* pAum = new AumentoRaio(t);
			cPartesCorpo.push_back(pAum);
			pAum = nullptr;
		}
			break;
		case TRANSICAODIMINUI:
		{
			DiminuiRaio t(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
			arq.read(reinterpret_cast<char*>(&t), sizeof(DiminuiRaio));
			DiminuiRaio* pDim = new DiminuiRaio(t);
			cPartesCorpo.push_back(pDim);
			pDim = nullptr;
		}
		break;
		}
	}

	for (size_t i = 0; i < FI.cNumConjAletas; i++)
	{
		switch (cTipoAletas[i])
		{
		case TRAPEZOIDAL:
		{
			Trapezoidal trap(0.0, 0.0, 0, 0.0, 0.0, Aleta::ARREDONDADO, 0.0, 0.0, 0.0, 0.0, 0.0);
			arq.read(reinterpret_cast<char*>(&trap), sizeof(Trapezoidal));
			Trapezoidal* pTrap = new Trapezoidal(trap);
			cAletas.push_back(pTrap);
			pTrap = nullptr;
		}			
			break;
		}
	}

	char* pMotorPath = new char[FI.cNumCharsMotorPath + 1];
	pMotorPath[FI.cNumCharsMotorPath] = 0;

	arq.read(pMotorPath, FI.cNumCharsMotorPath * sizeof(char));
	
	cPMotor = new Motor(pMotorPath, FI.cPosTopoMotor);

	if (!cPMotor->Valido())
	{
		//return false;
	}

	delete[] pMotorPath;
	pMotorPath = nullptr;

	char* pNomeFoguete = new char[FI.cNumCharsNomeFoguete + 1];
	pNomeFoguete[FI.cNumCharsNomeFoguete] = 0;

	arq.read(pNomeFoguete, FI.cNumCharsNomeFoguete * sizeof(char));

	cNomeFoguete = pNomeFoguete;

	delete[]pNomeFoguete;
	pNomeFoguete = nullptr;

	Paraquedas paraquedas(0.0, 0.0, 0.0, 0.0, 0.0);
	arq.read(reinterpret_cast<char*>(&paraquedas), sizeof(Paraquedas));

	cPParaquedas = new Paraquedas(paraquedas);
	
	arq.close();

	CalcularDadosFoguete();

	return true;
}

Foguete::FILEINFO::FILEINFO(uint32_t numPartesCorpo, uint32_t numConjAletas, uint32_t numCharsMotorPath, uint32_t numCharsNomeFoguete, double posMotor)
	:
	cNumPartesCorpo(numPartesCorpo),
	cNumConjAletas(numConjAletas),
	cNumCharsMotorPath(numCharsMotorPath),
	cNumCharsNomeFoguete(numCharsNomeFoguete),
	cPosTopoMotor(posMotor)
{}

double Foguete::PegarMassaFoguete(double tempo) const
{
	return cMassaFogueteComMotor - cPMotor->PegMassa(0.0) + cPMotor->PegMassa(tempo);
}