#pragma once
#include "GraphicsDxD.h"
#include "Keyboard.h"
#include "Mouse.h"

#include "Font.h"

#include "RocketSim.h"
#include "RocketModel.h"
#include "PushButton.h"
#include "AdderButton.h"
#include "TextBox.h"

#include "Camera3D.h"
#include "RocketModelBuilder.h"

#include "Menu.h"
#include "RadioTuple.h"
#include "UIImage.h"
#include "TextBox.h"
#include "NumBox.h"
#include "AdderButton.h"
#include "PushButton.h"
#include "EventButton.h"
#include "SwitchButton.h"
#include "ScrollerSelector.h"
#include "TextDrawer.h"

#include "Arrow.h"
#include "Planet.h"
#include "Foguete.h"

#include "Cylinder.h"

enum MEN_ID;

class RocketBuilder
{
public:
	RocketBuilder(KeyboardClient& kbd, MouseClient& mouse, GraphicsDxD* pGfx, MEN_ID* pMen);
	~RocketBuilder();

	void Tick();

private:
	void Draw();
	void Update();

private:
	template<class T>
	void Delete(T** p);

private:
	void ClearZeros(std::string* pText)
	{
		pText->erase(pText->find_last_not_of('0') + 1, std::string::npos);
		if (pText->back() == '.')
			pText->push_back('0');
	}

private:
	enum TIPO
	{
		MOUSEPRESS,
		MOUSERELEASE,
		CHARPRESS
	};
private:
	void SubMenuCoifaDraw();
	void SubMenuCorpoDraw();
	void SubMenuAletaDraw();

	void SubMenuMotorDraw();
	void SubMenuParaqueDraw();
	void SubMenuOpcoesDraw();

	void SubMenuCoifaUpdateRocket();
	void SubMenuCorpoUpdateRocket();
	void SubMenuAletaUpdateRocket();

	void SubMenuMotorUpdateRocket();
	void SubMenuParaqueUpdateRocket();
	void SubMenuOpcoesUpdateRocket();

	void HandleSubCoifa(TIPO t, const DirectX::XMFLOAT2& mousPos, char key);
	void HandleSubCorpo(TIPO t, const DirectX::XMFLOAT2& mousPos, char key);
	void HandleSubAleta(TIPO t, const DirectX::XMFLOAT2& mousPos, char key);
	void HandleSubMotor(TIPO t, const DirectX::XMFLOAT2& mousPos, char key);
	void HandleSubParaq(TIPO t, const DirectX::XMFLOAT2& mousPos, char key);
	void HandleSubOptio(TIPO t, const DirectX::XMFLOAT2& mousPos, char key);

	void SaveFile(const std::string& filePathName);

public:
	void OpenFile(const std::string& filePathName);

private:
	void UpdateRocketData();

private:
	const DirectX::XMFLOAT2 DEFWNDDIMEN = { 1920.0f, 1080.0f };
	GraphicsDxD *cPGraphics;
	KeyboardClient &cKbd;
	MouseClient& cMouse;
	MEN_ID* cPMenuId;

	Camera3D cCamera3D;

	Font cConsolasBranca1324;
	Font cConsolasPreta1324;
	TextDrawer cConsolasDrawerBranca1324;
	TextDrawer cConsolasDrawerPreta1324;

	RocketModelBuilder *cPRocketModel;

	UIImage cBackgroundTemplate;
		
	TextBox *cPNomeFoguete;

	EventButton *cPZoomFitBut;

	PushButton *cPBotMenuPrincipal;
	PushButton *cPBotSimulacoes;
	PushButton *cPBotFoguetes;
	PushButton *cPBotStudio;
	
	Menu *cPMenuEsq;
	Menu *cPMenuDir;
	
	UIImage cBackgroundSubCoifa;
	NumBox *cPRaioInterCoif;
	NumBox *cPRaioExterCoif;
	NumBox *cPAlturaCoif;
	NumBox *cPDensidadeCoif;
	NumBox *cPRugosMediaCoif;
	RadioTuple *cPTipoCoifa;

	UIImage cBackgroundSubCorpo;
	UIImage cBackgroundSubTubo;
	UIImage cBackgroundSubTransicao;
	ScrollerSelector* cPSelectorPartesCorpo;

	EventButton* cPButAddTubo;
	EventButton* cPButAddTran;
	EventButton* cPButDelPart;

	NumBox *cPRaioIntTopoTrans;
	NumBox *cPRaioIntBaseTrans;
	NumBox *cPRaioExtTopoTrans;
	NumBox *cPRaioExtBaseTrans;
	NumBox *cPAlturaTrans;
	NumBox *cPDensidadeTrans;
	NumBox *cPRugosMediaTrans;

	NumBox *cPRaioIntTubo;
	NumBox *cPRaioExtTubo;
	NumBox *cPAlturaTubo;
	NumBox *cPDensidadeTubo;
	NumBox *cPRugosMediaTubo;

	UIImage cBackgroundSubAletas;
	NumBox *cPNumAletas;
	NumBox *cPCordaRaizAle;
	NumBox *cPCordaPontaAle;
	NumBox *cPDeslocCordaPontaAle;
	NumBox *cPEnvergaduraAle;
	NumBox *cPPosVerticalAle;
	NumBox *cPEspessuraAle;
	NumBox *cPRugosidadeAle;
	NumBox *cPDensidadeAle;

	UIImage cBackgroundSubMotor;
	TextBox *cPNomeArqMotor;
	EventButton* cPSearchMotorFileBut;

	UIImage cBackgroundSubParaq;
	NumBox *cPBoxDiamParaq;
	NumBox *cPBoxCDParaq;
	NumBox *cPBoxPosVertParaq;
	NumBox *cPBoxMassaParaq;
	NumBox *cPBoxTempoEjecaoParaq;

	bool cArquivoMotorNaoEncontrado;
	std::string cUltNomeMotor;

	UIImage cBackgroundSubOpt;
	EventButton* cPButAbrirFoguete;
	EventButton* cPButReAbrirFoguete;
	EventButton* cPButSalvarFoguete;
	EventButton* cPButSalvarComoFoguete;

	DirectX::XMFLOAT2 cPosMouseAgora;

	float cZoom;
	float cRot;
	DirectX::XMFLOAT2 cPosXY;

	bool cBotDirPress;

	DirectX::XMFLOAT2 cUltPosMouseDirPress;

	bool cBotMiddlePress;

	DirectX::XMFLOAT2 cUltPosMouseMiddlePress;

	Foguete cFogueteData;

	Planet cPosCPEsf;
	Planet cPosCGEsf;

	Arrow cFlechaA;
	Arrow cFlechaV;
	Arrow cFlechaR;

	std::string cFogPathOpen;
};