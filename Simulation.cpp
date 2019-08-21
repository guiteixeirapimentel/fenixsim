#include "Simulation.h"
#include <string>

#include "PushButton.h"

Simulation::Simulation(HWND hWnd, KeyboardClient& kbd, MouseClient& mouse, GraphicsDxD* pGfx)
	:
	cPGraphics(pGfx),
	cKbd(kbd),
	cMouse(mouse),
	cFontConsolas(L"imgs/Consolas13x24 Anti-Aliasing-Crisp-Orange.png", { 13.0f, 24.0f }, pGfx),
	cNFrames(0),
	cFrameTime(0.0f),
	cUpdateTime(0.0f),
	cGraphTime(0.0f),
	cPos({ 0.0f, 0.0f, 0.0f }),
	cPLua(2.0f, 100, 100, L"imgs/moonText.jpg", cPGraphics),
	cPTerra(2.0f, 100, 100, L"imgs/earthText.jpg", cPGraphics),
	cTheta(0.0f),
	cPhi(0.0f),
	cFlechaA(L"imgs/azul.png", pGfx),
	cFlechaV(L"imgs/vermelho.png", pGfx),
	cFlechaR(L"imgs/rosa.png", pGfx),
	cPRockSimulation(nullptr),
	cPRockModel(nullptr),
	cii(0)
{		
	cCamera3D.SetPosition(0.0f, 0.0f, -5.0);
	posCam.z = -20.0;
	cCamera2D.SetPosition(0.0f, 0.0f);
	cTimerFrame.Tick();
	cTimer.Tick();

	cSun.cAmbient = { 0.1f, 0.1f, 0.1f, 0.1f };
	cSun.cDiffuse = { 0.5f, 0.5f, 0.5f, 0.5f };
	cSun.cSpecular = { 0.6f, 0.6f, 0.6f, 0.6f };
	cSun.cAtt = { 3.0e-7f, 3.0e-7f, 3.0e-7f, 3.0e-7f };
	cSun.cPosition = { 0.0f, 0.0f, -3200.0f };
	cSun.range = FLT_MAX;

	cVelFog = {};
	cVelFog.y = 0.0001;

	RocketSimDesc rsd = {};

	//rsd.cRockData.cData.cCordaPontaAleta = 0.25;
	//rsd.cRockData.cData.cCordaRaizAleta = 0.20;
	//rsd.cRockData.cData.cDeslocamentoCordaRaizAletas = 0.05;
	//rsd.cRockData.cData.cEnvergaduraAletas = 0.15;
	//rsd.cRockData.cData.cAlturaCoifa = 0.3;
	//rsd.cRockData.cData.cRaioBaseCoifa = 0.055;
	//rsd.cRockData.cData.cRaioCorpo = 0.055;

	rsd.cPosInicial = { 0.0, 0.0, 0.0 };
	rsd.cAcelInicial = { 0.0, 0.0000001, 0.0 };
	const double angLancamentoGraus = 90.0;
	rsd.cAtiInicial = { cos(angLancamentoGraus * 3.14159 / 180.0), sin(angLancamentoGraus * 3.14159 / 180.0), 0.0 };
	rsd.cAtiInicial = rsd.cAtiInicial / Modulus(rsd.cAtiInicial);
	rsd.cVelInicial = { cos(angLancamentoGraus * 3.14159 / 180.0) *0.0000001, sin(angLancamentoGraus * 3.14159 / 180.0)*0.0000001, 0.0 };
	
	//rsd.cRockData.cData.cEspessuraAletas = 0.003;
	//rsd.cRockData.cData.cAlturaCorpo = 0.90;
	//rsd.cRockData.cData.cMassaSemCombustivel = 1.983 - 0.708;
	//rsd.cRockData.cData.cRugosidadeAleta = 150.0e-6;
	//rsd.cRockData.cData.cRugosidadeCoifa = 150.0e-6;
	//rsd.cRockData.cData.cRugosidadeCorpo = 150.0e-6;
	//rsd.cRockData.cData.cNumeroAletas = 4;
	//
	//rsd.cRockData.cData.cMomentoInerciaFoguete = 0.17;
	//
	cPRockSimulation = new RocketSim(rsd);
	
	cPRockSimulation->Simular(20.0, 0.0001);
	//cPRockModel = new RocketModel(rsd.cRockData, cPGraphics);
}

Simulation::~Simulation()
{
	if (cPRockModel)
	{
		delete cPRockModel;
		cPRockModel = nullptr;
	}
	if (cPRockSimulation)
	{
		delete cPRockSimulation;
		cPRockSimulation = nullptr;
	}
}

void Simulation::Tick()
{
	cTimerFrame.Tick();
	cTimer.Tick();
	
	cPGraphics->IniciarCena();
	Draw();	
	cPGraphics->TerminarCena();

	cTimer.Tick();
	cGraphTime = cTimer.DeltaTime();	

	cTimer.Tick();

	Update();

	cTimer.Tick();
	cUpdateTime = cTimer.DeltaTime();

	if (cNFrames < 32)
	{
		cNFrames++;
		cFrameTime += cTimerFrame.DeltaTime();
	}
	else
	{
		cNFrames = 0;
		cFrameTime = 0.0f;
	}
}

void Simulation::Draw()
{
	cCamera3D.Render();
	cCamera2D.Render();

	DirectX::XMMATRIX viewMatrix3D;
	cCamera3D.GetViewMatrix(viewMatrix3D);
	
	DirectX::XMMATRIX viewMatrix2D;
	cCamera2D.GetViewMatrix(viewMatrix2D);

	auto posCam = cCamera3D.GetPosition();
	DirectX::XMFLOAT4 eyePosW = { posCam.x, posCam.y, posCam.z, 0.0f };
	cPGraphics->SetLightAndEyePos(cSun, eyePosW);

	//cSun.cPosition = posCam;

	DirectX::XMFLOAT2 posMouse = { (float)cMouse.GetMouseX() ,(float)cMouse.GetMouseY() };

	cPGraphics->Enable3DAndLights();

	static float z = 0.0f;

	float scale = 3474.0f / 12742.0f;
	//cPLua.Render({ 200.0f, 0.0f, 0.0f }, { 0.0f, z, 0.0f }, 3474.0f, viewMatrix3D, cPGraphics);
	cPTerra.Render({ 0.0f, 0.0f, 100.0f }, { 0.0f, z, 0.0f }, 10.0f, viewMatrix3D, cPGraphics);

	z += 0.0001f;

	cPGraphics->Enable3D();	
	
	unsigned int index = unsigned int(((1.0 / cPRockSimulation->PegarDadosSim()->cDeltaTime) / 120.0) * cii);

	if (index < cPRockSimulation->PegarDadosSim()->cPosicoes.size() && index >= 0)
	{
		const auto poss = cPRockSimulation->PegarDadosSim()->cPosicoes[index];
		DirectX::XMFLOAT3 pos = { (float)poss.x, (float)poss.y, (float)poss.z };
		
		//cFlechaV.Render({}, cPRockSimulation->PegarDadosSim()->cAceleracoesAngulares[index], viewMatrix3D, cPGraphics);
		//cFlechaR.Render({}, cPRockSimulation->PegarDadosSim()->cVelocidadesAngulares[index], viewMatrix3D, cPGraphics);

		VETOR posLook = { pos.x - posCam.x, pos.y - posCam.y, pos.z - posCam.z };
		posLook /= Modulus(posLook);

		//cCamera3D.LookAt(posLook.x, posLook.y, posLook.z);

		const auto atii = cPRockSimulation->PegarDadosSim()->cAtitudes[index];
		DirectX::XMFLOAT3 ati = { (float)atii.x, (float)atii.y, (float)atii.z };

		//cFlechaR.Render({}, atii, viewMatrix3D, cPGraphics);

		cPRockModel->Raster(pos, ati, viewMatrix3D, cPGraphics);

		auto vv = cPRockSimulation->PegarDadosSim()->cVelocidades[index];		
		//cFlechaA.Render({}, vv, viewMatrix3D, cPGraphics);

		auto aa = cPRockSimulation->PegarDadosSim()->cAceleracoes[index];
		aa /= Modulus(aa);
		//cFlechaV.Render({}, aa, viewMatrix3D, cPGraphics);

		cPGraphics->Enable2D();
		//std::string txt = "Velocidade: ";
		//txt = txt + std::to_string(vv.x) + " " + std::to_string(vv.y) + " " + std::to_string(vv.z);
		
		/*cFontConsolas.Write(txt, { 0.0f, 0.0f }, { 13.0f,26.0 }, Font::CENTER, cPGraphics);
		cFontConsolas.Write("Index: "+std::to_string(cii), { 0.0f, 26.0f }, { 13.0f,26.0 }, Font::CENTER, cPGraphics);
		cFontConsolas.Write("Alpha: " + std::to_string(AngleBetween(vv, atii) * 180.0 / 3.14159), 
		{ 0.0f, 52.0f }, { 13.0f, 26.0f }, Font::CENTER, cPGraphics);
		cFontConsolas.Write("Posy: " + std::to_string(poss.y),
		{ 0.0f, 78.0f }, { 13.0f, 26.0f }, Font::CENTER, cPGraphics);*/
		cPGraphics->Enable3D();
	}
	
	//cFlecha.Render({ 0.0, 0.0, 0.0 }, { 1.0, 0.0, 0.0 }, viewMatrix3D, cPGraphics);
			
	cPGraphics->Enable2D();	

	/*std::string velF = "Vel: " + std::to_string(cVelFog.y);
	velF += " " + std::to_string(cRocket.GetCDS().x) + " " + std::to_string(cRocket.GetCDS().y) + " " + 
		std::to_string(cRocket.GetCDS().z) + ";";
	cFontConsolas.Write(velF, {-250.0f, 0.0f}, { 13.0f, 26.0f }, Font::CENTER, cPGraphics);*/

	std::string mousPos = "Pos mouse: ";
	mousPos += std::to_string(cMouse.GetMouseX()) + " " + std::to_string(cMouse.GetMouseY());
	cFontConsolas.Write(mousPos, { -200.0f, -300.0f }, { 13.0f, 26.0f }, Font::RIGHT, cPGraphics);

	//FPS
	std::string fps = "Secs/Frame: " + std::to_string(cFrameTime / cNFrames);
	cFontConsolas.Write(fps,
	{ cPGraphics->GetWndWidth()/2.0f , -(float)cPGraphics->GetWndHeight()/ 2.0f }, 
	{ 13.0f * 0.6f, 26.0f * 0.6f }, Font::RIGHT, cPGraphics);

	unsigned int framesPerSec = unsigned int(1.0f / (cFrameTime / cNFrames));

	fps = "FPS: " + std::to_string(framesPerSec);
	cFontConsolas.Write(fps,
	{ cPGraphics->GetWndWidth() / 2.0f, -(float)(cPGraphics->GetWndHeight() / 2.0f) + (26.0f * 0.6f) },
	{ 13.0f * 0.6f, 26.0f * 0.6f }, Font::RIGHT, cPGraphics);

	fps = "Time D3D: " + std::to_string(cGraphTime);
	cFontConsolas.Write(fps,
	{ cPGraphics->GetWndWidth() / 2.0f, -(float)(cPGraphics->GetWndHeight() / 2.0f) + (52.0f * 0.6f) },
	{ 13.0f * 0.6f, 26.0f * 0.6f }, Font::RIGHT, cPGraphics);

	fps = "Time Update: " + std::to_string(cUpdateTime);
	cFontConsolas.Write(fps,
	{ cPGraphics->GetWndWidth() / 2.0f, -(float)(cPGraphics->GetWndHeight() / 2.0f) + (78.0f * 0.6f) },
	{ 13.0f * 0.6f, 26.0f * 0.6f }, Font::RIGHT, cPGraphics);

}

void Simulation::Update()
{
	while (cKbd.PeekChar())
	{
		unsigned char k = cKbd.ReadChar();

	}
	
	auto me = cMouse.ReadMouse();
	if(me.IsValid())
	{
		if (me.GetType() == MouseEvent::LPress)
		{
			DirectX::XMFLOAT2 mousePos = { (float)me.GetX(), (float)me.GetY() };
		}
		else if(me.GetType() == MouseEvent::LRelease)
		{
			DirectX::XMFLOAT2 mousePos = { (float)me.GetX(), (float)me.GetY() };
		}
	}

	if (cKbd.KeyIsPressed(VK_UP) && cPhi < 3.14159f / 4.0f)
	{
		cPhi += 0.025f;
	}
	if (cKbd.KeyIsPressed(VK_DOWN)&& cPhi > -3.14159f/4.0f)
	{
		cPhi -= 0.025f;
	}
	if (cKbd.KeyIsPressed(VK_LEFT))
	{
		cTheta -= 0.025f;
	}
	if (cKbd.KeyIsPressed(VK_RIGHT))
	{
		cTheta += 0.025f;
	}
	
	float lx = sinf(cTheta);
	float ly = sinf(cPhi);
	float lz = cosf(cTheta);
	cCamera3D.LookAt(lx, ly, lz);

	if (cKbd.KeyIsPressed('W'))
	{
		posCam.x += lx / 10.0f;
		posCam.y += ly / 10.0f;
		posCam.z += lz / 10.0f;
	}
	if (cKbd.KeyIsPressed('S'))
	{
		posCam.x -= lx / 10.0f;
		posCam.y -= ly / 10.0f;
		posCam.z -= lz / 10.0f;
	}

	if (cKbd.KeyIsPressed('L'))
	{
		cii++;
	}
	if (cKbd.KeyIsPressed('K'))
	{
		cii--;
	}
	
	cCamera3D.SetPosition(posCam.x, posCam.y, posCam.z);
}
