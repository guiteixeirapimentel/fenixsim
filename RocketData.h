#pragma once
#include <string>

enum TIPOCOIFA
{
	CONICA,
	OGIVA
};

struct RD
{
	size_t cNomeFogueteSize;
	double cMassaSemCombustivel;
	double cCPXIniFoguete;

	//GEOMETRIA FOGUETE
	double cAreaLateralFoguete;
	double cVolumeFogueteAparente;
	double cAlturaFoguete;
	double cMomentoInerciaFoguete;
	double cCMXFoguete;

	//COIFA
	TIPOCOIFA cTipoCoifa;
	double cRaioBaseCoifa;
	double cRaioBaseCoifaInt;
	double cAlturaCoifa;
	double cRugosidadeCoifa;

	double cAreaBaseCoifa;
	double cAreaLateralCoifa;
	double cVolumeCoifaAparente;
	double cVolumeCoifaReal;
	double cDensidadeCoifa;
	double cMassaCoifa;
	double cCMXCoifa;


	//CORPO
	double cAlturaCorpo;
	double cRaioCorpo;
	double cRaioCorpoIn;
	double cRugosidadeCorpo;

	double cAreaBaseCorpo;
	double cAreaLateralCorpo;
	double cVolumeCorpoAparente;
	double cVolumeCorpoReal;
	double cDensidadeCorpo;
	double cMassaCorpo;
	double cCMXCorpo;

	double cXCPCorpoECoifa;

	//ALETAS
	//        |\        |
	//  F     | \       |
	//  O     |  \      | m = Deslocamento corda Raiz
	//  G     |   \     |
	//  U  cr |\   \    |  cr= corda raiz aleta
	//  E     | \   \   | 
	//  T     |  \l  | l = comprimento envergadura aleta
	//  E     |   \  |
	//         \   \ |         
	//          \   \|           
	//           \   | ct=corda ponta aleta       
	//            \  |          
	//             \ |            
	//              \|                
	//^       --------
	//^           s = envergadura aletas
	//^
	//x y >>>>
	//
	//
	//
	unsigned int cNumeroAletas;
	double cCordaRaizAleta;
	double cCordaPontaAleta;
	double cPosicaoYAletas; //em relação a ponta da coifa.
	double cEspessuraAletas;
	double cDeslocamentoCordaRaizAletas;
	double cEnvergaduraAletas;
	double cComprimentoEnvergaduraAleta;
	double cRugosidadeAleta;
	double cDensidadeAleta;
	double cVolumeAleta;
	double cMassaAleta;
	double cCMXAleta;

	double cAreaLateralAleta;
	double cTauLAletas;
	double cTauCAletas;
	double cARAletas;
	double cCPXAletas;
	double cCPYAletas;

	double cFinenessRatioCorpo;
	double cFinenessRatioCoifa;// cFb;
	double cFinenessRatioRocket;

	size_t cPathFileMotorSize;
};

class RocketData
{
public:
	RocketData();

	~RocketData();

	bool Open(const std::string& filePathName);
	void Save(const std::string& filePathName);

	RD GetData() const
	{
		return cData;
	}
	
public:
	std::string cNomeFoguete;
	std::string cPathFileMotor;

	RD cData;
};