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
#include "RocketSim.h"

#include "Menu.h"
#include "RadioTuple.h"
#include "UIImage.h"
#include "TextBox.h"
#include "NumBox.h"
#include "AdderButton.h"
#include "PushButton.h"
#include "EventButton.h"
#include "SwitchButton.h"
#include "TextDrawer.h"

#include "Arrow.h"
#include "Planet.h"
#include "RocketData.h"

#include "Foguete.h"

#include "RocketSim.h"
#include "SimDataExporter.h"

enum MEN_ID;

class RocketSimulator
{
public:
	RocketSimulator(KeyboardClient& kbd, MouseClient& mouse, GraphicsDxD* pGfx, MEN_ID* pMen);
	~RocketSimulator();

	void Tick();

private:
	void Draw();
	void Update();

private:
	template<class T>
	void Delete(T** p);

private:
	enum TIPO
	{
		MOUSEPRESS,
		MOUSERELEASE,
		CHARPRESS
	};

private:
	void SubMenuWeatherDraw();
	void SubMenuInfoDraw();
	void SubMenuSimDraw();

	void SubMenuWeatherUpdateRocket();
	void SubMenuInfoUpdateRocket();
	void SubMenuSimUpdateRocket();

	void HandleSubWeather(TIPO t, const DirectX::XMFLOAT2& mousPos, char key);
	void HandleSubInfo(TIPO t, const DirectX::XMFLOAT2& mousPos, char key);
	void HandleSubSim(TIPO t, const DirectX::XMFLOAT2& mousPos, char key);

private:
	void SaveFile(const std::string& filePathName) const;
	void OpenFile(const std::string& filePathName);

private:
	void ClearZeros(std::string* pText)
	{
		pText->erase(pText->find_last_not_of('0') + 1, std::string::npos);
		if (pText->back() == '.')
			pText->push_back('0');
	}
private:
	const DirectX::XMFLOAT2 DEFWNDDIMEN = { 1920.0f, 1080.0f };
	GraphicsDxD *cPGraphics;
	KeyboardClient &cKbd;
	MouseClient& cMouse;
	MEN_ID* cPMenuId;

	SimDataExporter cDataExporter;

	Camera3D cCamera3D;

	Font cConsolasBranca1324;
	Font cConsolasPreta1324;

	TextDrawer* cPWhiteFontDrawer;

	RocketModelBuilder *cPRocketModel;

	Menu *cPMenuDir;

	UIImage cBackgroundTemplate;
	UIImage cBackgroundSubEsq;
	UIImage cBackgroundSubDir;

	TextBox *cPBoxNomeSimulacao;

	EventButton *cPBotZoomFit;

	PushButton *cPBotMenuPrincipal;
	PushButton *cPBotSimulacoes;
	PushButton *cPBotFoguetes;
	PushButton *cPBotStudio;

	NumBox* cPBoxCompHaste;
	NumBox* cPBoxAngLanc;
	NumBox* cPBoxDeltaT;

	RadioTuple* cPRadSimAte;
	NumBox* cPBoxTempoSim;

	TextBox *cPBoxNomeArqFog;
	EventButton* cPBotSearchFoguete;
	EventButton* cPBotRefreshFoguete;
	std::string cUltNomeArqFoguete;

	EventButton* cPBotSimular;
	EventButton* cPBotSalvar;
	EventButton* cPBotAbrir;

	EventButton* cPBotExPos;
	EventButton* cPBotExVels;
	EventButton* cPBotExAccel;
	EventButton* cPBotExAtit;
	EventButton* cPBotExVelsAng;
	EventButton* cPBotExAccsAng;

	UIImage cBackgroundSubWeather;
	NumBox* cPBoxAltitudeLocal;
	NumBox* cPBoxTemperaturaLocal;

	bool cFogueteEncontrado;
		
	DirectX::XMFLOAT2 cPosMouseAgora;

	float cZoom;
	float cRot;
	DirectX::XMFLOAT2 cPosXY;

	bool cBotDirPress;

	DirectX::XMFLOAT2 cUltPosMouseDirPress;

	bool cBotMiddlePress;

	DirectX::XMFLOAT2 cUltPosMouseMiddlePress;

	Foguete cFogueteData;
	RocketSim* cPSimulation;
	bool cSimulado;

	Arrow cFlechaA;
	Arrow cFlechaV;
	Arrow cFlechaR;
};