#pragma once
#include "GraphicsDxd.h"
#include "LinAlg.h"
#include "Texture.h"
#include "Cylinder.h"
#include "FinModel.h"

#include <vector>

constexpr double MASSAESPECIFICAAR = 1.2928;
constexpr double VISCOSIDADEAR = 1.5*10e-5;
constexpr double VELOCIDADESOM = 340.0;

class Rocket
{
public:
	Rocket(GraphicsDxD* pGfx);
	~Rocket();

	void Tick();

	void Raster(const DirectX::XMMATRIX& viewMatrix, GraphicsDxD* cPGraphics) const;
	void DrawInfo(const class Font& font, GraphicsDxD* pGraphics) const;

public:
	enum nosetype
	{
		PARABOLA,
		OGIVE,
		CONE
	};

public:
	void SetRocketPos(VETOR pos)
	{
		cPos = pos;
	}

public:
	double CalcCNAlphaBodySubsonic() const;
	double CalcCMAlphaBodySubsonic() const;

	void SetVel(const VETOR& v)
	{
		cVel = v;
	}

	VETOR GetCDS() const
	{
		return { Cd_Coifa, Cd_Total, Cd_Pressure };
	}


private:
	double CalcCLCorrectionBody() const;	 

	//Calcular CD, empuxo, peso.
	//Calcular CN/CM resto do foguete
	double CalcCfCSkinFriction() const;
	double CalcCdFriction() const;

	double CalcCdPressure() const;

private:
	//Parametros de referencia

	const double REFAREA = 1.0;
	const double REFLEN = 1.0;

	//Parâmetros fisicos

	VETOR cPos;
	VETOR cVel;
	VETOR cAcc;
	
	VETOR cAti;

	double cAlpha;

	double cMach;
	double cReynolds;
	double cReynoldsCrit;
	double cReynoldsSonic;

	double cMassaTotal;

	//Geometria do Foguete

	nosetype cNoseType;
	double cNoseBaseRadius;
	double cNoseHeight;
	double cNoseLateralArea;

	double cBodyHeight;
	double cBodyRadius;

	double cBodyLateralArea;
	double cBodyVolume;
	double cBodyIntegralCorpo;
	double cAreaBase;

	double cYOffsetAletas;
	unsigned int cNumFins;
	double cFinThickness;
	double cFinChordTrailing;
	double cFinChordRoot;
	double cFinM;
	double cFinSpan;
	double cFinLength;
	double cFinTauL;
	double cFinTauC;
	double cFinAR;
	double cFinXCP;
	double cFinYCP;
	double cFinLateralArea;

	const double cRugosityBody;
	
	// Coeficientes Aerodinamicos

	//Partes

	//Body
	double cNAlphaBody; //x
	double CMAlphaBody; //x
	double cNAlphaBodyLiftCorrection; //x
	
	double cCPBody;
	double cBodyDfb;


	//Fins
	double cCNAlphaFins;
	double cNAlphaFinsBodyInterfCorrection;
	
	double cCPFins;

	//Drag

	double cCfSkin;
	double cCfSonicSkin;
	double cFCompressibilityCorrections;
	double cDFriction;

	double cDragFins;

	double cFb;

	double Cd_Pressure;
	double Cd_Total;
	double Cd_Aleta;
	double Cd_Coifa;

private:
	double cForcasArrasto;

private:
	Cylinder cBodyModel;
	Cylinder cNoseCoseModel;
	FinModel cFinModel;

	Texture cBodyTexture;
	Texture cNoseConeTexture;
	Texture cFinTexture;
};