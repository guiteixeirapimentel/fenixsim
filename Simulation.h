#pragma once
#include "GraphicsDxD.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Camera3D.h"
#include "Camera2D.h"

#include "Font.h"

#include "Timer.h"

#include "TensionCube.h"
#include "CylinderBody.h"

#include "Image.h"

#include "Planet.h"

#include "Arrow.h"

#include "RocketSim.h"
#include "RocketModel.h"

#include "PushButton.h"
#include "AdderButton.h"
#include "TextBox.h"

class Simulation
{
public:
	Simulation(HWND hWnd, KeyboardClient& kbd, MouseClient& mouse, GraphicsDxD* pGfx);
	~Simulation();

	void Tick();
	
private:
	void Draw();
	void Update();

private:
	GraphicsDxD *cPGraphics;
	KeyboardClient &cKbd;
	MouseClient& cMouse;

	Font cFontConsolas;

	Camera3D cCamera3D;
	Camera2D cCamera2D;
	
	Timer cTimerFrame;
	Timer cTimer;

	float cFrameTime;
	float cGraphTime;
	float cUpdateTime;
	uint32_t cNFrames;
	
	VETOR cPos;

	Planet cPLua;
	Planet cPTerra;
	
	float cTheta;
	float cPhi;

	DirectX::XMFLOAT3 posCam;
	
	Arrow cFlechaA;
	Arrow cFlechaV;
	Arrow cFlechaR;

	PointLight cSun;

	VETOR cVelFog;

	RocketSim *cPRockSimulation;
	RocketModel *cPRockModel;

	int cii;
};