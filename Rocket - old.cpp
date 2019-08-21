#include "Rocket - old.h"
#include <math.h>
#include "Font.h"

constexpr double PI = 3.14159265359;

Rocket::Rocket(GraphicsDxD* pGfx)
	:
	cPos({ 0.0, 0.0, -3200.0 }),
	cVel({ 0.0, 0.0, 0.0 }),
	cAcc({ 0.0, 0.0, 0.0 }),
	cAti({ 0.0, 0.0, 0.0 }),
	cRugosityBody(150.0e-6),
	cBodyModel(0.04f, 0.04f, 0.9f, 100, 100, pGfx->GetPDevice()),
	cNoseCoseModel(0.04f, 0.0f, 0.3f, 100, 100, pGfx->GetPDevice()),
	cFinModel(0.25f, 0.15f, 0.05f, 0.20f, 0.003f, pGfx->GetPDevice()),
	cBodyTexture(L"imgs/textCylinder.png", pGfx->GetPDevice(), pGfx->GetPDeviceContext()),
	cNoseConeTexture(L"imgs/noseConeTex.png", pGfx->GetPDevice(), pGfx->GetPDeviceContext()),
	cFinTexture(L"imgs/finText.png", pGfx->GetPDevice(), pGfx->GetPDeviceContext()),
	cNumFins(4),
	cMassaTotal(2.442)
{
	cFinChordTrailing = 0.15;
	cFinChordRoot = 0.25;
	cFinM = 0.05;
	cFinSpan = 0.40;
	cFinLength = sqrt((cFinSpan*cFinSpan) + pow((0.5*(cFinChordTrailing - cFinChordRoot) + cFinM), 2));

	cFinLateralArea = (cFinChordTrailing + cFinChordRoot)*cFinSpan * 0.5;
	cFinTauL = atan(cFinM / cFinSpan);
	cFinTauC = acos(cFinSpan / cFinLength);
	cFinAR = cFinLateralArea / cFinChordRoot;
	cFinXCP = (2.0 * cBodyRadius) + ((1.0 / 6.0)*(cFinChordRoot + cFinChordTrailing + 
		((cFinChordRoot*cFinChordTrailing) / (cFinChordRoot + cFinChordTrailing))
		));
	cFinYCP = cBodyRadius + ((cFinSpan / 3.0)*(cFinChordRoot + (2.0 * cFinChordTrailing)) / (cFinChordRoot + cFinChordRoot));
	
	cFinXCP = (cBodyHeight + cNoseHeight) - cFinSpan + cFinXCP;

	cReynoldsCrit = 51.0 * pow(cRugosityBody / cBodyHeight + cNoseHeight, -1.039);

	cFb = (cBodyHeight + cNoseHeight) / (2.0 * cBodyRadius);

	cReynoldsSonic = (MASSAESPECIFICAAR * (cBodyHeight + cNoseHeight) * VELOCIDADESOM) / VISCOSIDADEAR;

	cNoseHeight = 0.3;
	cNoseBaseRadius = cBodyRadius = 0.01;

	cFinThickness = 0.003;

	cBodyHeight = 0.45;

	cAreaBase = cBodyRadius * cBodyRadius * PI;

	cBodyLateralArea = (cBodyHeight + cNoseHeight) * PI * cBodyRadius * 2.0;
	cBodyVolume = PI * (cBodyRadius * cBodyRadius) * (cBodyHeight + cNoseHeight);

	cNoseLateralArea = PI * (cNoseBaseRadius * cNoseBaseRadius) * cNoseHeight / 3.0;
}

Rocket::~Rocket()
{}

void Rocket::Tick()
{	
	constexpr VETOR paraCima = { 0.0, 1.0, 0.0 };

	cAlpha = AngleBetween(cAti, paraCima);

	const double Awb = (cBodyHeight + cNoseHeight) * cBodyRadius * 2.0;
	
	const double modVelocidade = Modulus(cVel);
	const double comprTotalFoguete = cNoseHeight + cBodyHeight;

	cFb = (cNoseHeight + cBodyHeight) / (2.0*cBodyRadius);

	cMach = modVelocidade / VELOCIDADESOM;
	cReynolds = (MASSAESPECIFICAAR * comprTotalFoguete * modVelocidade) / VISCOSIDADEAR;	
	cReynoldsSonic = (MASSAESPECIFICAAR * comprTotalFoguete * 340.0) / VISCOSIDADEAR;
	cReynoldsCrit = 51.0 * pow((cRugosityBody / comprTotalFoguete), -1.039);

	if (cReynolds < 5.0e5)
	{
		cCfSkin = 0.0148;
	}
	else if (cReynolds > cReynoldsCrit)
	{
		cCfSkin = 0.032*pow((cRugosityBody / comprTotalFoguete), 0.2);
	}
	else
	{
		cCfSkin = 1.0 / pow((1.5 * log(cReynolds) - 5.6), 2);
	}

	double CfSkinC = 0.0;
	//1.1 Compressible subsonic variation
	if (cMach < 1.0)
	{
		if (cReynolds < 5.0e5)
		{
			CfSkinC = cCfSkin * (1.0f - (0.09 * cMach * cMach));

		}
		else
		{
			CfSkinC = cCfSkin * (1.0f - (0.12 * cMach * cMach));
		}
	}
	else
	{
		if (cReynolds < 5.0e5)
		{
			CfSkinC = cCfSkin / pow((1.0f - (0.045 * cMach * cMach)), 0.25);

		}
		else
		{
			CfSkinC = cCfSkin / pow((1.0f - (0.15 * cMach * cMach)), 0.58);
		}
	}

	cDragFins = 2.0 * cNumFins * CfSkinC *(cFinLateralArea / REFAREA);

	cBodyDfb = (1.0 + (0.5 / cFb)) * CfSkinC * cBodyLateralArea / REFAREA;

	const double cCdfCoifa = CfSkinC * cNoseLateralArea / REFAREA;

	cDFriction = cBodyDfb + cDragFins;
	
	double Cf = 0.0;

	// cálculo do Cfc_sonic
	if (cReynoldsSonic < 5.0e5)
	{
		Cf = 1.328 / sqrt(cReynoldsSonic);
	}			
	else if (cReynoldsSonic > cReynoldsCrit)
	{
		Cf = 0.032*pow((cRugosityBody / (cBodyHeight + cNoseHeight)), 0.2);
	}		
	else
	{
		Cf = (1.0 / pow((3.46*log(cReynoldsCrit) - 5.6), 2)) - (1700.0 / cReynolds);
	}		
	if (cReynoldsSonic < 5.0*(10e5))
	{
		cCfSonicSkin = Cf / pow((1 - 0.045), 1.0 / 4.0);
	}		
	else
	{
		cCfSonicSkin = Cf / pow(1 - 0.15, 0.58);
	}			
	
	// 2) Pressure drag
	double deltacd = 0.0;
	// 2.1) Tail pressure drag
	if (cMach < 0.9)
	{
		deltacd = pow(1.0 - (cMach*cMach), -0.417) - 1.0;
	}
	else if (cMach >= 0.9 && cMach <= 1.0)
	{
		deltacd = 1.0 - (1.5 * (cMach - 0.9));
	}
	else
	{
		deltacd = 1.214 - (0.502 / (cMach * cMach)) + (0.1095 / (cMach * cMach * cMach * cMach)) + (0.0231 / (cMach * cMach * cMach * cMach * cMach * cMach));
	}

	//Leading Edge Drag
	
	//fin leading edge radius
	double sr = cFinThickness / 2.0;

	double cd_L = 2.0 * cNumFins * (sr / REFAREA) * (pow(cos(cFinTauL), 2.0)* deltacd);
	
	//The trailing edge is zero because it's sharp

	//thickness drag

	double epsilon = cFinAR * pow((cFinThickness * cFinChordRoot), 1.0 / 3.0);
	double cdTh1 = 1.15 * (pow(cFinThickness / cFinChordRoot, 5.0 / 3.0) * (1.61 + epsilon - sqrt(pow(epsilon - 1.43, 2) + 0.578)));
	double k1 = ((cdTh1 * REFAREA /(cCfSonicSkin * cFinSpan * cFinThickness)) 
		- 4.0*(cFinThickness / cFinChordRoot)*cos(cFinTauC)) / (120.0 * (pow(cFinThickness/cFinChordRoot, 4) * 
		(pow(cos(cFinTauC), 2))));
	double k = pow(cos(cFinTauC), 2) + pow(k1, 2.0 / 3.0);

	double cCdTh = 0.0;
	if (cMach < 1.0)
	{
		cCdTh = CfSkinC * (cFinSpan * cFinThickness / REFAREA) * (4.0 * (cFinThickness / cFinChordRoot) *cos(cFinTauC) +
			120 * pow(cFinThickness / cFinChordRoot, 4)*pow(cos(cFinTauC), 2) / (k - (cMach * pow(cos(cFinTauC), 2))));
	}
	else
	{
		cCdTh = 4 * cNumFins * CfSkinC * (cFinSpan * cFinThickness / REFAREA) * (
			(cFinThickness / cFinChordRoot) * cos(cFinTauC) + 30.0*(pow(cFinThickness / cFinChordRoot, 4)*pow(cos(cFinTauC), 2)
				/ (k - (cMach * pow(cos(cFinTauC), 2)))));
	}

	//2.2) Body pressure drag

	const double a = 0.88;
	const double b = 0.7;
	const double c = 1.29;

	double Cd_p = a / pow(((cNoseHeight / (2.0*cNoseBaseRadius)) + b), c);
	
	double kk = 1.0 + (6 * Awb *
		cCfSonicSkin / pow((pow((cBodyHeight + cNoseHeight) / (2.0*cBodyRadius), 3) * REFAREA / Cd_p), 5.0/3.0));

	const double fb = (cNoseHeight + cBodyHeight) / (2.0*cBodyRadius);

	Cd_p = 6.0 * Awb * cCfSkin / (pow(fb, 3) * REFAREA * pow((kk - (cMach * cMach)), 0.6));


	double Cd_b1 = 0.185; // hr = 0 + 1.15*hr/cr

	double kkk = 1.0 + (0.029 * 0.029) / (cCfSonicSkin*(Awb / REFAREA)*(Cd_b1*Cd_b1));
	double Cd_b = 0.0;

	if (cMach < 1.0)
	{
		Cd_b = (0.029*cAreaBase/ REFAREA) / sqrt(cCfSonicSkin * (Awb / REFAREA) * (kkk - (cMach*cMach)));
	}
	else
	{
		Cd_b = Cd_b1 * (0.88 + (0.12*exp(-3.58*(cMach - 1.0))));
	}

	Cd_Pressure = cd_L + cCdTh + Cd_p + Cd_b;

	Cd_Total = cDFriction + Cd_Pressure;

	Cd_Aleta = cDragFins + cd_L + cCdTh;

	Cd_Coifa = cCdfCoifa + Cd_p;

	cForcasArrasto = Cd_Total * REFAREA * 0.5 * MASSAESPECIFICAAR * modVelocidade * modVelocidade;
	VETOR velUnit = { cVel.x, cVel.y, cVel.z };

	if (modVelocidade != 0.0)
	{
		velUnit /= modVelocidade;
	}
	else
	{

		velUnit = { 0.0, 0.0, 0.0 };
	}
	static const double dt = 0.0001;
	static double simTime = 0.0;

	VETOR cAccArrasto = { -velUnit.x, -velUnit.y, -velUnit.z };
	cAccArrasto *= cForcasArrasto / cMassaTotal;

	VETOR g = { 0.0, -9.78, 0.0 };

	cAcc = { 0.0, 0.0, 0.0 };

	cAcc += g;
	cAcc += cAccArrasto;

	if (simTime < 1.497)
	{
		cAcc.y += 125.0 / cMassaTotal;
	}
	else if (simTime < 2.902)
	{
		cAcc.y += 18.0 / cMassaTotal;
	}
	else
	{
		cMassaTotal = 2.442 - (0.708 - 0.345);
	}
	
	cVel += cAcc * dt;

	static double hmax = 0.0;
	const double h = cPos.y;

	if (h > hmax)
	{
		hmax = h;
	}

	static double velMax = 0.0;

	if (cVel.y > velMax)
	{
		velMax = cVel.y;
	}

	static double acelMax = 0.0;

	if (cAcc.y > acelMax)
	{
		acelMax = cAcc.y;
	}

	cPos += cVel * dt;

	simTime += dt;
	//assert(!_DEBUG);
}

void Rocket::Raster(const DirectX::XMMATRIX& viewMatrix, GraphicsDxD* cPGraphics) const
{
	//Render BODY MODEL
	ID3D11Buffer* pVertexBuffer = cBodyModel.GetVertexBuffer();
	ID3D11Buffer* pIndexBuffer = cBodyModel.GetIndexBuffer();

	DirectX::XMVECTOR v = { (float)cAti.x, (float)cAti.y, (float)cAti.z, (float)1.0 };

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixRotationRollPitchYawFromVector(v) *
		DirectX::XMMatrixTranslation((float)cPos.x, (float)cPos.y, (float)cPos.z);

	cPGraphics->RenderModel3D(viewMatrix, worldMatrix, cPGraphics->GetProjMatrix(), cBodyTexture.GetTexture(), pVertexBuffer,
		pIndexBuffer, cBodyModel.GetIndexCount());

	//RENDER NOSE CONE MODEL
	pVertexBuffer = cNoseCoseModel.GetVertexBuffer();
	pIndexBuffer = cNoseCoseModel.GetIndexBuffer();
	
	worldMatrix = DirectX::XMMatrixTranslation(0.0f, float(cBodyHeight + cNoseHeight)/2.0f, 0.0f) *
		DirectX::XMMatrixRotationRollPitchYawFromVector(v) *
		DirectX::XMMatrixTranslation((float)cPos.x, (float)cPos.y, (float)cPos.z);

	cPGraphics->RenderModel3D(viewMatrix, worldMatrix, cPGraphics->GetProjMatrix(), cNoseConeTexture.GetTexture(), 
		pVertexBuffer, pIndexBuffer, cNoseCoseModel.GetIndexCount());

	//RENDER FINS

	const float passo = (2.0f * (float)PI) / static_cast<float>(cNumFins);
	for (unsigned int i = 0; i < cNumFins; i++)
	{
		pVertexBuffer = cFinModel.GetVertexBuffer();
		pIndexBuffer = cFinModel.GetIndexBuffer();
		
		DirectX::XMMATRIX adjust = DirectX::XMMatrixRotationY(static_cast<float>(i) * passo)
			* DirectX::XMMatrixTranslation((float)cBodyRadius * cosf(static_cast<float>(i) * passo), 
				-float(cBodyHeight-cFinChordRoot)/2.0f, (float)-cBodyRadius * sinf(static_cast<float>(i) * passo) );

		worldMatrix = adjust * DirectX::XMMatrixRotationRollPitchYawFromVector(v) *
			DirectX::XMMatrixTranslation((float)cPos.x, (float)cPos.y, (float)cPos.z);

		cPGraphics->RenderModel3D(viewMatrix, worldMatrix, cPGraphics->GetProjMatrix(), cFinTexture.GetTexture(), pVertexBuffer,
			pIndexBuffer, cFinModel.GetIndexCount());
	}	
}

void Rocket::DrawInfo(const Font& font, GraphicsDxD* pGraphics) const
{
	std::string dadosFoguete = "Vel: " + std::to_string(Modulus(cVel)) + "; Accel " + std::to_string(Modulus(cAcc));
	font.Write(dadosFoguete, {}, { 13.0f, 26.0f }, Font::CENTER, pGraphics);

	dadosFoguete = "Pos Y: " + std::to_string(cPos.y);
	font.Write(dadosFoguete, { 0.0f, 27.0f }, { 13.0f, 26.0f }, Font::CENTER, pGraphics);
}

double Rocket::CalcCNAlphaBodySubsonic() const
{
	/*const double A1 = PI * r1 * r1;
	const double A0 = PI * r0 * r0;
	double CNA;

	if (cAlpha != 0.0)
		CNA = (2.0*(A1 - A0) * sin(cAlpha)) / (refArea * cAlpha);
	else
		CNA = (2.0*(A1 - A0)) / refArea;*/

	const double CNA = CalcCLCorrectionBody();

	return CNA;
}

double Rocket::CalcCMAlphaBodySubsonic() const
{
	double CMA;
	if (cAlpha != 0.0)
		CMA = (2.0 * cBodyHeight*((PI*cBodyRadius*cBodyRadius) - cBodyVolume) * sin(cAlpha)) / (REFAREA*REFLEN * cAlpha);
	else
		CMA = (2.0 * cBodyHeight*((PI*cBodyRadius*cBodyRadius) - cBodyVolume)) / (REFAREA*REFLEN);

	return CMA;
}

double Rocket::CalcCLCorrectionBody() const
{
	constexpr double K = 1.1;
	const double sinAlpha = sin(cAlpha);
	double CL = (K * (cBodyHeight * 2.0 * cBodyHeight) * (sinAlpha * sinAlpha)) / REFAREA;

	return CL;
}

double Rocket::CalcCfCSkinFriction() const
{
	const double velFoguete = Modulus(cVel);
	const double Mach = velFoguete / 340.0;
	const double R = (MASSAESPECIFICAAR * velFoguete * REFLEN) / VISCOSIDADEAR;

	/*
	RS MÉDIO EM MICROM = 10E-6
	Average glass 0.1
	Finished and polished surface 0.5
	Optimum paint-sprayed surface 5
	Planed wooden boards 15
	Paint in aircraft mass production 20
	Smooth cement surface 50
	Dip-galvanized metal surface 150
	Incorrectly sprayed aircraft paint 200
	Raw wooden boards 500
	Average concrete surface 1000
	*/
	constexpr double RS = 15 * 10e-6;

	const double RCrit = 51.0 * pow((RS / (cNoseHeight + cBodyHeight)), -1.039);

	double cf = 0.0;

	if (R < 10e4)
	{
		cf = 1.48*10e-2;
	}
	else if(R < RCrit)
	{
		double den = ((1.5* log(R)) - 5.6);
		den *= den;
		cf = 1.0 / den;
	}
	else
	{
		cf = 0.032*pow(RS / (cNoseHeight + cBodyHeight),0.2);
	}

	if (Mach < 0.8)
	{
		cf *= 1.0 - (0.1 * Mach * Mach);
	}
	else
	{
		cf /= pow(1 + (0.15*Mach*Mach), 0.58);
	}

	return cf;
}

double Rocket::CalcCdFriction() const
{
	const double fb = (cBodyHeight+cNoseHeight) / (cBodyRadius*2.0);

	double cfc = CalcCfCSkinFriction();

	const double areaWettedBody = 2.0 * PI * cBodyRadius * cBodyHeight;
	const double areaWettedFins = 000;

	const double meanAerodynamicChordLengthFins = 00;

	double CdFric = cfc *
		(((1.0 + (0.5/fb)) * areaWettedBody) +
		(1.0 + (2.0 * cFinThickness / meanAerodynamicChordLengthFins))) / REFAREA;

	return CdFric;
}

double Rocket::CalcCdPressure() const
{
	const double velRocket = Modulus(cVel);
	const double Mach = velRocket / 340.0;

	const double thetaCoifa = atan(cNoseBaseRadius / cNoseHeight);

	double tmp = sin(thetaCoifa);
	tmp *= tmp;

	const double noseConeCdp = 0.8 * tmp;

	//Não esquecer conversão de REFAREA
	double cdFinPressure = 0.0;

	//Considerando leading edge sem arredondamento
	if (Mach < 1.0)
	{
		cdFinPressure = 1.0 + (Mach * Mach / 4.0) + (pow(Mach, 4.0) / 40.0);
	}
	else
	{
		cdFinPressure = 1.84 - (0.76 / (Mach*Mach)) + (0.166 / pow(Mach, 4.0)) + (0.035 / pow(Mach, 6.0));
	}

	//Adicionando trailing edge effect on pressure drag
	if (Mach < 1.0)
	{
		cdFinPressure += 0.12 + (0.13 * Mach * Mach);
	}
	else
	{
		cdFinPressure += 0.25 / Mach;
	}

	cdFinPressure = 000 * cdFinPressure / REFAREA;

	double cdBasePressureDrag = 0.0;

	if (Mach < 1.0)
	{
		cdBasePressureDrag = 0.12 + (0.13 * Mach * Mach);
	}
	else
	{
		cdBasePressureDrag = 0.25 / Mach;
	}

	return cdBasePressureDrag + cdFinPressure + noseConeCdp;
}