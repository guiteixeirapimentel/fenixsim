#include "RocketSimulator.h"
#include "Program.h"
#include "WinWrappers.h"

RocketSimulator::RocketSimulator(KeyboardClient& kbd, MouseClient& mouse, GraphicsDxD* pGfx, MEN_ID* pMen)
	:
	cPGraphics(pGfx),
	cKbd(kbd),
	cMouse(mouse),
	cPMenuId(pMen),
	cConsolasBranca1324(L"Assets/Fontes/Consolas13x24Branca.png", { 13.0f, 24.0f }, cPGraphics),
	cConsolasPreta1324(L"Assets/Fontes/Consolas13x24Preta.png", { 13.0f, 24.0f }, cPGraphics),
	cPRocketModel(nullptr),
	cPSimulation(nullptr),
	cSimulado(false),
	cBackgroundTemplate(L"Assets/UI/RocketSim/background.png", { 1920.0f, 1080.0f }, { 0.0f, 0.0f }, DEFWNDDIMEN, cPGraphics),
	cPMenuDir(nullptr),
	cPBoxNomeSimulacao(nullptr),
	cPBotZoomFit(nullptr),
	cPBotMenuPrincipal(nullptr),
	cPBotFoguetes(nullptr),
	cPBotSimulacoes(nullptr),
	cPBotStudio(nullptr),
	cPBoxTempoSim(nullptr),
	cPBoxAngLanc(nullptr),
	cPBoxCompHaste(nullptr),
	cPBoxDeltaT(nullptr),
	cPRadSimAte(nullptr),
	cPBotExPos(nullptr),
	cPBotExVels(nullptr),
	cPBotExAccel(nullptr),
	cPBotExAtit(nullptr),
	cPBotExVelsAng(nullptr),
	cPBotExAccsAng(nullptr),
	cPWhiteFontDrawer(nullptr),
	cPBoxAltitudeLocal(nullptr),
	cPBoxTemperaturaLocal(nullptr),
	cBackgroundSubEsq(L"Assets/UI/RocketSim/backgroundSubEsq.png", { 465.0f, 780.0f }, { 69.0f, 120.0f }, DEFWNDDIMEN, cPGraphics),
	cBackgroundSubDir(L"Assets/UI/RocketSim/backgroundSubDir.png", { 204.0f, 316.0f }, { 1386.0f, 325.0f }, DEFWNDDIMEN, cPGraphics),
	cBackgroundSubWeather(L"Assets/UI/RocketSim/backgroundSubWeather.png", { 482.0f, 132.0f }, {1378.0f, 232.0f}, DEFWNDDIMEN, cPGraphics),
	cPBoxNomeArqFog(nullptr),
	cPBotRefreshFoguete(nullptr),
	cPBotSearchFoguete(nullptr),
	cUltNomeArqFoguete(""),
	cFogueteEncontrado(false),
	cZoom(-2.0f),
	cRot(0.0f),
	cBotDirPress(false),
	cBotMiddlePress(false),
	cFlechaA(L"imgs/azul.png", pGfx),
	cFlechaV(L"imgs/vermelho.png", pGfx),
	cFlechaR(L"imgs/rosa.png", pGfx)
{
	cCamera3D.SetPosition(cPosXY.x, cPosXY.y, cZoom);
	cPosMouseAgora = {};
	cUltPosMouseDirPress = {};
	cUltPosMouseMiddlePress = {};
	cPosXY = {};
	
	cPRocketModel = new RocketModelBuilder(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, cPGraphics);

	ButtonDesc butD = {};
	butD.defWndSize = DEFWNDDIMEN;

	butD.fileNameDefault = L"Assets/UI/RocketSim/botMenPrinDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/botMenPrinPress.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/botMenPrinRoll.png";

	butD.pos = { 63.0f, 992.0f };
	butD.size = { 442.0f, 70.0f };

	cPBotMenuPrincipal = new PushButton(butD, cPGraphics);

	butD.pos = { 520.0f, 992.0f };

	butD.fileNameDefault = L"Assets/UI/RocketSim/botMenSimDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/botMenSimPress.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/botMenSimRoll.png";

	cPBotSimulacoes = new PushButton(butD, cPGraphics);

	butD.pos = { 976.0f, 992.0f };

	butD.fileNameDefault = L"Assets/UI/RocketSim/botMenFogDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/botMenFogPress.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/botMenFogRoll.png";

	cPBotFoguetes = new PushButton(butD, cPGraphics);

	butD.pos = { 1432.0f, 992.0f };

	butD.fileNameDefault = L"Assets/UI/RocketSim/botMenStuDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/botMenStuPress.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/botMenStuRoll.png";

	cPBotStudio = new PushButton(butD, cPGraphics);

	
	BoxDesc boxd = {};
	boxd.defWndSize = DEFWNDDIMEN;
	boxd.pFont = &cConsolasBranca1324;
	
	boxd.centralizado = true;
	boxd.fileNameDefault = L"Assets/UI/RocketSim/nSimDef.png";
	boxd.fileNameSelected = L"Assets/UI/RocketSim/nSimSel.png";
	boxd.fontSize = { 36.0f, 67.0f };
	boxd.pos = { 649.0f, 6.0f };
	boxd.size = { 619.0f, 89.0f };

	cPBoxNomeSimulacao = new TextBox(boxd, cPGraphics);

	boxd.centralizado = false;
	boxd.pFont = &cConsolasPreta1324;
	boxd.fontSize = { 15.17f, 28.0f };
	boxd.fileNameDefault = L"Assets/UI/RocketSim/textBoxDef.png";
	boxd.fileNameSelected = L"Assets/UI/RocketSim/textBoxSel.png";

	boxd.size = { 140.0f, 52.0f };

	boxd.pos = { 341.0f, 115.0f };
	cPBoxCompHaste = new NumBox(boxd, cPGraphics);
	cPBoxCompHaste->SetText("1.0");

	boxd.pos = { 341.0f, 235.0f };
	cPBoxAngLanc = new NumBox(boxd, cPGraphics);
	cPBoxAngLanc->SetText("90.0");

	boxd.pos = { 341.0f, 345.0f };
	cPBoxDeltaT = new NumBox(boxd, cPGraphics);

	cPBoxDeltaT->SetText("0.001");

	boxd.pos = { 1665.0f, 222.0f };
	cPBoxAltitudeLocal = new NumBox(boxd, cPGraphics);
	cPBoxAltitudeLocal->SetText("852.19");

	boxd.pos = { 1665.0f, 312.0f };
	cPBoxTemperaturaLocal = new NumBox(boxd, cPGraphics);
	cPBoxTemperaturaLocal->SetText("28.0");

	cPRadSimAte = new RadioTuple();	

	butD.fileNameDefault = L"Assets/UI/RocketSim/radioDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/radioDef.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/radioDef.png";
	butD.size = { 80.0f, 60.0f };
	
	butD.pos = { 140.0f, 532.0f };
	cPRadSimAte->AddButton(butD, L"Assets/UI/RocketBuilder/radioChecked.png", cPGraphics);

	butD.pos = { 361.0f, 532.0f };
	cPRadSimAte->AddButton(butD, L"Assets/UI/RocketBuilder/radioChecked.png", cPGraphics);

	butD.pos = { 140.0f, 696.0f };
	cPRadSimAte->AddButton(butD, L"Assets/UI/RocketBuilder/radioChecked.png", cPGraphics);

	boxd.pos = { 296.0f, 768.0f };
	boxd.size = { 140.0f, 52.0f };

	cPBoxTempoSim = new NumBox(boxd, cPGraphics);

	boxd.fileNameDefault = L"Assets/UI/RocketBuilder/textboxlargdef.png";

	boxd.pos = { 75.0f, 912.0f };
	boxd.size = { 360.0f, 48.0f };

	cPBoxNomeArqFog = new TextBox(boxd, cPGraphics);

	butD.fileNameDefault = L"Assets/UI/RocketSim/botBuscDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/botBuscPress.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/botBuscRoll.png";

	butD.size = { 50.0f, 46.0f };
	butD.pos = { 446.0f, 913.0f };
	cPBotSearchFoguete = new EventButton(butD, cPGraphics);

	butD.fileNameDefault = L"Assets/UI/RocketSim/refreshDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/refreshPress.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/refreshRoll.png";

	butD.size = { 50.0f, 46.0f };
	butD.pos = { 504.0f, 913.0f };

	cPBotRefreshFoguete = new EventButton(butD, cPGraphics);
	
	butD.size = { 150.0f, 66.0f };

	butD.fileNameDefault = L"Assets/UI/RocketSim/simDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/simPress.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/simRoll.png";

	butD.pos = { 1386.0f, 199.0f };
	cPBotSimular = new EventButton(butD, cPGraphics);
	
	butD.fileNameDefault = L"Assets/UI/RocketSim/saveDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/savePress.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/saveRoll.png";

	butD.pos = { 1550.0f, 199.0f };
	cPBotSalvar = new EventButton(butD, cPGraphics);

	butD.fileNameDefault = L"Assets/UI/RocketSim/openDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/openPress.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/openRoll.png";

	butD.pos = { 1714.0f, 199.0f };
	cPBotAbrir = new EventButton(butD, cPGraphics);
	
	butD.size = { 150.0f, 69.0f };

	butD.fileNameDefault = L"Assets/UI/RocketSim/posDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/posPress.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/posRoll.png";

	butD.pos = { 1390.0f, 664.0f };
	cPBotExPos = new EventButton(butD, cPGraphics);

	butD.fileNameDefault = L"Assets/UI/RocketSim/velDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/velPress.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/velRoll.png";

	butD.pos = { 1552.0f, 664.0f };

	cPBotExVels = new EventButton(butD, cPGraphics);

	butD.fileNameDefault = L"Assets/UI/RocketSim/acelDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/acelPress.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/acelRoll.png";

	butD.pos = { 1714.0f, 664.0f };

	cPBotExAccel = new EventButton(butD, cPGraphics);

	butD.fileNameDefault = L"Assets/UI/RocketSim/atiDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/atiPress.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/atiRoll.png";

	butD.pos = { 1390.0f, 764.0f };

	cPBotExAtit = new EventButton(butD, cPGraphics);

	butD.fileNameDefault = L"Assets/UI/RocketSim/velAngDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/velAngPress.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/velAngRoll.png";

	butD.pos = { 1552.0f, 764.0f };

	cPBotExVelsAng = new EventButton(butD, cPGraphics);

	butD.fileNameDefault = L"Assets/UI/RocketSim/acelAngDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/acelAngPress.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/acelAngRoll.png";

	butD.pos = { 1714.0f, 764.0f };

	cPBotExAccsAng = new EventButton(butD, cPGraphics);

	cPWhiteFontDrawer = new TextDrawer(cConsolasBranca1324, boxd.defWndSize);

	cPMenuDir = new Menu();

	butD.size = { 124.0f, 78.0f };

	butD.fileNameDefault = L"Assets/UI/RocketSim/botMenuWeatherDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/botMenuWeatherPress.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/botMenuWeatherRoll.png";
	butD.pos = { 1371.0f, 79.0f };

	cPMenuDir->AddButton(butD, L"Assets/UI/RocketSim/botMenuWeatherSwitch.png", cPGraphics);

	butD.fileNameDefault = L"Assets/UI/RocketSim/botMenuInfoDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/botMenuInfoPress.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/botMenuInfoRoll.png";
	butD.pos = { 1560.0f, 79.0f };

	cPMenuDir->AddButton(butD, L"Assets/UI/RocketSim/botMenuInfoSwitch.png", cPGraphics);

	butD.fileNameDefault = L"Assets/UI/RocketSim/botMenuSimDef.png";
	butD.fileNamePressed = L"Assets/UI/RocketSim/botMenuSimPress.png";
	butD.filenameRollover = L"Assets/UI/RocketSim/botMenuSimRoll.png";
	butD.pos = { 1746.0f, 79.0f };

	cPMenuDir->AddButton(butD, L"Assets/UI/RocketSim/botMenuSimSwitch.png", cPGraphics);


	cPBoxNomeSimulacao->SetText("Simulacao 00");
}

RocketSimulator::~RocketSimulator()
{
	Delete(&cPRocketModel);
	Delete(&cPBotZoomFit);
	Delete(&cPBoxNomeSimulacao);

	Delete(&cPBotFoguetes);
	Delete(&cPBotMenuPrincipal);
	Delete(&cPBotSimulacoes);
	Delete(&cPBotStudio);

	Delete(&cPWhiteFontDrawer);

	Delete(&cPBoxAltitudeLocal);
	Delete(&cPBoxTemperaturaLocal);

	Delete(&cPBoxNomeArqFog);
	Delete(&cPBotSearchFoguete);
	Delete(&cPBotRefreshFoguete);

	Delete(&cPSimulation);

	Delete(&cPMenuDir);

	Delete(&cPBoxTempoSim);

	Delete(&cPBotSimular);
	Delete(&cPBotSalvar);
	Delete(&cPBotAbrir);

	Delete(&cPBotExPos);
	Delete(&cPBotExVels);
	Delete(&cPBotExAccel);
	Delete(&cPBotExAtit);
	Delete(&cPBotExVelsAng);
	Delete(&cPBotExAccsAng);

	Delete(&cPBoxAngLanc);
	Delete(&cPBoxCompHaste);
	Delete(&cPRadSimAte);
	Delete(&cPBoxDeltaT);
}

void RocketSimulator::Tick()
{
	cPGraphics->IniciarCena();
	Draw();
	cPGraphics->TerminarCena();

	Update();
}

void RocketSimulator::Draw()
{
	cCamera3D.Render();
	const DirectX::XMFLOAT4 posEye = { cCamera3D.GetPosition().x, cCamera3D.GetPosition().y, cCamera3D.GetPosition().z, 1.0f };
	PointLight pl;

	pl.cAmbient = { 0.5f, 0.5f, 0.5f, 1.0f };
	pl.cDiffuse = { 0.5f, 0.5f, 0.5f, 1.0f };
	pl.cSpecular = { 1.0f, 1.0f, 1.0f, 1.0f };
	pl.cAtt = { 0.0f, 0.01f, 0.0f, 1.0f };
	pl.cPosition = { 50.0f, 50.0f, -50.0f };
	pl.range = FLT_MAX;

	cPGraphics->SetLightAndEyePos(pl, posEye);

	DirectX::XMMATRIX viewMatrix3D;
	cCamera3D.GetViewMatrix(viewMatrix3D);

	auto posCam = cCamera3D.GetPosition();

	cPGraphics->Enable3DAndLights();

	size_t it = 0;

	if (cFogueteEncontrado)
	{
		const unsigned int numFins = cFogueteData.cAletas[0]->PegarNumAletas();
		const float alturaCorpo = (float)cFogueteData.cPartesCorpo[1]->PegarAltura();
		const float alturaCoifa = (float)cFogueteData.cPartesCorpo[0]->PegarAltura();
		const float raioMaximo = (float)cFogueteData.cRaioMaximo;
		const float raioBaseCoifa = (float)cFogueteData.cPartesCorpo[0]->PegarRaioBaseExt();
		const float deslAletBaseCorpo = (float)(cFogueteData.cAletas[0]->PegarPosV());
		const float rot = cRot;

		DirectX::XMFLOAT3 ati = { 0.0f, 1.0f, 0.0f };

		if (cSimulado)
		{
			static float t = 0.0f;

			if (t + 0.01666f < cPSimulation->PegarDadosSim()->cTempoTotal)
			{
				t += 0.01666f;
			}
			else
			{
				t = 0.0f;
			}

			it = static_cast<size_t>((t / cPSimulation->PegarDadosSim()->cDeltaTime) - 0.5f);
			VETOR att = cPSimulation->PegarDadosSim()->cAtitudes[it];
			ati = {float(att.x), float(att.y), float(att.z)};
		}

		cPRocketModel->Raster(cFogueteData, 0.0f, rot, ati, viewMatrix3D, cPGraphics);

		const DirectX::XMFLOAT3 posCp = { -2.5f * raioMaximo,
			(float)(-cFogueteData.cPosVCPFoguete), 0.0f };
		static float rrr = 0.0f;
		rrr += 0.01f;
		//cPosCPEsf.Render(posCp, { 0.0f, rrr, 0.0f }, raioCorpo / 1.2f, viewMatrix3D, cPGraphics);

		if (cFogueteData.cPosVCPFoguete != 0.0 && raioMaximo != 0.0)
			cFlechaA.Render({ double(-3.5f * raioMaximo), 0.0, 0.0 },
			{ 0.0, -cFogueteData.cPosVCPFoguete, 0.0 },
				viewMatrix3D, cPGraphics);

		const VETOR posCoord = { 1.5 * raioMaximo, 0.0, 0.0 };

		cFlechaR.Render(posCoord, { -alturaCorpo / 5.0, 0.0, 0.0 }, viewMatrix3D, cPGraphics);
		cFlechaR.Render(posCoord, { 0.0, -alturaCorpo / 5.0, 0.0 }, viewMatrix3D, cPGraphics);
		cFlechaR.Render(posCoord, { 0.0, 0.0, -alturaCorpo / 5.0 }, viewMatrix3D, cPGraphics);
	}	

	cPGraphics->Enable2D();
	cBackgroundTemplate.Draw(cPGraphics);
	cBackgroundSubEsq.Draw(cPGraphics);

	cPMenuDir->Draw(cPosMouseAgora, cPGraphics);

	cPBotMenuPrincipal->Draw(cPosMouseAgora, cPGraphics);
	cPBotSimulacoes->Draw(cPosMouseAgora, cPGraphics);
	cPBotFoguetes->Draw(cPosMouseAgora, cPGraphics);
	cPBotStudio->Draw(cPosMouseAgora, cPGraphics);

	cPBoxNomeSimulacao->Draw(cPosMouseAgora, cPGraphics);

	cPBoxCompHaste->Draw(cPosMouseAgora, cPGraphics);
	cPBoxAngLanc->Draw(cPosMouseAgora, cPGraphics);
	cPBoxDeltaT->Draw(cPosMouseAgora, cPGraphics);

	cPRadSimAte->Draw(cPosMouseAgora, cPGraphics);
	cPBoxTempoSim->Draw(cPosMouseAgora, cPGraphics);
	
	cPBoxNomeArqFog->Draw(cPosMouseAgora, cPGraphics);
	cPBotSearchFoguete->Draw(cPosMouseAgora, cPGraphics);
	cPBotRefreshFoguete->Draw(cPosMouseAgora, cPGraphics);

	switch (cPMenuDir->GetActiveButton())
	{
	case 0:
	{
		SubMenuWeatherDraw();
	}
		break;
	case 1:
	{
		SubMenuInfoDraw();
	}
		break;
	case 2:
	{
		SubMenuSimDraw();
	}
		break;
	}
	

	if (cSimulado)
	{
		std::string buf;
		
		// INFOS SIM REAL

		buf = "Tempo:" + std::to_string(int(it * cPSimulation->PegarDadosSim()->cDeltaTime * 10.0) / 10.0);
		ClearZeros(&buf);
		buf += " s";
		cPWhiteFontDrawer->DrawString(buf, { 653.0f, 722.0f }, 50.0f, cPGraphics);

		buf = "Vel:" + std::to_string(int(Modulus(cPSimulation->PegarDadosSim()->cVelocidades[it]) * 10.0) / 10.0);
		ClearZeros(&buf);
		buf += " m/s";
		cPWhiteFontDrawer->DrawString(buf, { 653.0f, 771.0f }, 50.0f, cPGraphics);

		buf = "Pos V.:" + std::to_string(int(cPSimulation->PegarDadosSim()->cPosicoes[it].y * 10.0) / 10.0);
		ClearZeros(&buf);
		buf += " m";
		cPWhiteFontDrawer->DrawString(buf, { 653.0f, 819.0f }, 50.0f, cPGraphics);

		double distLateral = sqrt(pow(cPSimulation->PegarDadosSim()->cPosicoes[it].x, 2) + pow(cPSimulation->PegarDadosSim()->cPosicoes[it].z, 2));

		buf = "Dis H.:" + std::to_string(int(distLateral * 10.0) / 10.0);
		ClearZeros(&buf);
		buf += " m";
		cPWhiteFontDrawer->DrawString(buf, { 653.0f, 868.0f }, 50.0f, cPGraphics);

		buf = "Alpha:" + std::to_string(cPSimulation->PegarDadosSim()->cAlphas[it] * 180.0 / 3.14159);//int(cPSimulation->PegarDadosSim()->cAlphas[it] * 10.0) / 10.0);
		ClearZeros(&buf);
		buf += " graus";
		cPWhiteFontDrawer->DrawString(buf, { 653.0f, 918.0f }, 50.0f, cPGraphics);
	}
}

void RocketSimulator::Update()
{
	cPosMouseAgora = { float(cMouse.GetMouseX()), float(cMouse.GetMouseY()) };

	if (cBotDirPress && cMouse.RightIsPressed())
	{
		float deltaX = cUltPosMouseDirPress.x - cPosMouseAgora.x;

		cRot += deltaX / 100.f;
	}

	if (cMouse.RightIsPressed())
	{
		cUltPosMouseDirPress = cPosMouseAgora;
		cBotDirPress = true;
	}
	else
	{
		cBotDirPress = false;
	}

	if (cBotMiddlePress && cMouse.WheelIsPressed())
	{
		float deltaX = cUltPosMouseMiddlePress.x - cPosMouseAgora.x;
		float deltaY = cUltPosMouseMiddlePress.y - cPosMouseAgora.y;
		deltaX /= 100.0f;
		deltaY /= 100.0f;

		if (cPosXY.x + deltaX < 3.14159f / 4.0f && cPosXY.x + deltaX > -3.14159f / 4.0f)
			cPosXY.x += deltaX;
		if (cPosXY.y - deltaY  < 3.14159f / 4.0f && cPosXY.y - deltaY > -3.14159f / 4.0f)
			cPosXY.y -= deltaY;
	}

	if (cMouse.WheelIsPressed())
	{
		cUltPosMouseMiddlePress = cPosMouseAgora;
		cBotMiddlePress = true;
	}
	else
	{
		cBotMiddlePress = false;
	}

	if (cPBotMenuPrincipal->AlreadyPushed())
	{
		*cPMenuId = MEN_MAN;
		cPBotMenuPrincipal->ReSet();
	}
	if (cPBotFoguetes->AlreadyPushed())
	{
		*cPMenuId = MEN_BUI;
		cPBotFoguetes->ReSet();
	}
	if (cPBotSimulacoes->AlreadyPushed())
	{
		*cPMenuId = MEN_SIM;
		cPBotSimulacoes->ReSet();
	}
	if (cPBotStudio->AlreadyPushed())
	{
		*cPMenuId = MEN_STU;
		cPBotStudio->ReSet();
	}

	while (cKbd.PeekChar())
	{
		unsigned char k = cKbd.ReadChar();

		cPBoxNomeSimulacao->KeyPress(k);

		cPBoxCompHaste->KeyPress(k);
		cPBoxAngLanc->KeyPress(k);
		cPBoxTempoSim->KeyPress(k);
		cPBoxNomeArqFog->KeyPress(k);
		cPBoxDeltaT->KeyPress(k);

		switch (cPMenuDir->GetActiveButton())
		{
		case 0:
		{
			HandleSubWeather(CHARPRESS, {}, k);
		}
		break;
		case 1:
		{
			HandleSubInfo(CHARPRESS, {}, k);
		}
		break;
		case 2:
		{
			HandleSubSim(CHARPRESS, {}, k);
		}
		break;
		}
	}

	auto me = cMouse.ReadMouse();
	if (me.IsValid())
	{
		if (me.GetType() == MouseEvent::LPress)
		{
			DirectX::XMFLOAT2 mousePos = { (float)me.GetX(), (float)me.GetY() };
			cPBoxNomeSimulacao->MousePress(mousePos, cPGraphics);

			cPMenuDir->MousePress(mousePos, cPGraphics);

			cPBotFoguetes->MousePress(mousePos, cPGraphics);
			cPBotMenuPrincipal->MousePress(mousePos, cPGraphics);
			cPBotSimulacoes->MousePress(mousePos, cPGraphics);
			cPBotStudio->MousePress(mousePos, cPGraphics);
			
			cPBoxCompHaste->MousePress(mousePos, cPGraphics);
			cPBoxAngLanc->MousePress(mousePos, cPGraphics);
			cPBoxTempoSim->MousePress(mousePos, cPGraphics);
			cPBoxNomeArqFog->MousePress(mousePos, cPGraphics);
			cPBoxDeltaT->MousePress(mousePos, cPGraphics);

			cPBotSearchFoguete->MousePress(mousePos, cPGraphics);
			cPBotRefreshFoguete->MousePress(mousePos, cPGraphics);
			cPRadSimAte->MousePress(mousePos, cPGraphics);

			switch (cPMenuDir->GetActiveButton())
			{
			case 0:
			{
				HandleSubWeather(MOUSEPRESS, mousePos, 0);
			}
			break;
			case 1:
			{
				HandleSubInfo(MOUSEPRESS, mousePos, 0);
			}
			break;
			case 2:
			{
				HandleSubSim(MOUSEPRESS, mousePos, 0);
			}
			break;
			}			

		}
		else if (me.GetType() == MouseEvent::LRelease)
		{
			DirectX::XMFLOAT2 mousePos = { (float)me.GetX(), (float)me.GetY() };

			cPBotFoguetes->MouseRelease(mousePos, cPGraphics);
			cPBotMenuPrincipal->MouseRelease(mousePos, cPGraphics);
			cPBotSimulacoes->MouseRelease(mousePos, cPGraphics);
			cPBotStudio->MouseRelease(mousePos, cPGraphics);

			cPMenuDir->MouseRelease(mousePos, cPGraphics);
			
			cPBotSearchFoguete->MouseRelease(mousePos, cPGraphics);
			cPBotRefreshFoguete->MouseRelease(mousePos, cPGraphics);

			cPRadSimAte->MouseRelease(mousePos, cPGraphics);
						
			switch (cPMenuDir->GetActiveButton())
			{
			case 0:
			{
				HandleSubWeather(MOUSERELEASE, mousePos, 0);
			}
			break;
			case 1:
			{
				HandleSubInfo(MOUSERELEASE, mousePos, 0);
			}
			break;
			case 2:
			{
				HandleSubSim(MOUSERELEASE, mousePos, 0);
			}
			break;
			}
		}
		else if (me.GetType() == MouseEvent::WheelUp)
		{
			float d = (0.05f * float(cFogueteData.cAlturaFoguete / 0.9f));
			if (cZoom + d < 0.0f)
				cZoom += d;
		}
		else if (me.GetType() == MouseEvent::WheelDown)
		{
			float d = (0.05f * float(cFogueteData.cAlturaFoguete / 0.9f));
			cZoom -= d;
		}
	}

	if (cPBotSearchFoguete->ReturnLastState())
	{
		cSimulado = false;
		const std::string path = GetPathFromOpenFileA("FENIXR\0*.fog\0\0");
		cPBoxNomeArqFog->SetText(path);

		if (cFogueteData.Abrir(path))
		{
			cPRocketModel->DeletarTodasTransicoes();

			for (size_t i = 0; i < cFogueteData.cTipoPartes.size(); i++)
			{
				switch (cFogueteData.cTipoPartes[i])
				{
				case Foguete::TUBO:
				{}
				break;
				case Foguete::TRANSICAOAUMENTO:
				case Foguete::TRANSICAODIMINUI:
				{
					cPRocketModel->AdicionarTransicao((float)cFogueteData.cPartesCorpo[i]->PegarAltura(),
						(float)cFogueteData.cPartesCorpo[i]->PegarRaioBaseExt(), (float)cFogueteData.cPartesCorpo[i]->PegarRaioTopoExt(),
						i, cPGraphics);
				}
				break;
				case Foguete::COIFACONICA:
				{}
				break;
				}
			}
			
			cPRocketModel->SetAletaInfo((float)cFogueteData.cAletas[0]->PegarCordaRaiz(), (float)cFogueteData.cAletas[0]->PegarCordaPonta(),
				(float)cFogueteData.cAletas[0]->PegarDeslCordaPonta(), (float)cFogueteData.cAletas[0]->PegarEnvergadura(),
				(float)cFogueteData.cAletas[0]->PegarEspessura(), cPGraphics);

			cFogueteEncontrado = true;		
		}		
		else
		{
			cFogueteEncontrado = false;			
		}
	}

	if (cPBotRefreshFoguete->ReturnLastState())
	{
		cSimulado = false;

		const std::string path = cPBoxNomeArqFog->GetText();

		if (cFogueteData.Abrir(path))
		{
			cPRocketModel->DeletarTodasTransicoes();

			for (size_t i = 0; i < cFogueteData.cTipoPartes.size(); i++)
			{
				switch (cFogueteData.cTipoPartes[i])
				{
				case Foguete::TUBO:
				{}
				break;
				case Foguete::TRANSICAOAUMENTO:
				case Foguete::TRANSICAODIMINUI:
				{
					cPRocketModel->AdicionarTransicao((float)cFogueteData.cPartesCorpo[i]->PegarAltura(),
						(float)cFogueteData.cPartesCorpo[i]->PegarRaioBaseExt(), (float)cFogueteData.cPartesCorpo[i]->PegarRaioTopoExt(),
						i, cPGraphics);
				}
				break;
				case Foguete::COIFACONICA:
				{}
				break;
				}
			}

			cPRocketModel->SetAletaInfo((float)cFogueteData.cAletas[0]->PegarCordaRaiz(), (float)cFogueteData.cAletas[0]->PegarCordaPonta(),
				(float)cFogueteData.cAletas[0]->PegarDeslCordaPonta(), (float)cFogueteData.cAletas[0]->PegarEnvergadura(),
				(float)cFogueteData.cAletas[0]->PegarEspessura(), cPGraphics);

			cFogueteEncontrado = true;
		}
		else
		{
			cFogueteEncontrado = false;
		}
	}

	if (cPBotSimular->ReturnLastState() && cFogueteEncontrado)
	{
		RocketSimDesc rsd = {};

		const double angle = std::atof(cPBoxAngLanc->GetText().c_str()) * 3.14159 / 180.0;

		rsd.cAtiInicial = {cos(angle), sin(angle), 0.0f};

		rsd.cPosInicial = { 0.0, 0.0, 0.0 };
		rsd.cVelInicial = { 0.0, 0.0, 0.0 };
		rsd.cAcelInicial = { 0.0, 0.0, 0.0 };
		rsd.cComprimentoHaste = std::atof(cPBoxCompHaste->GetText().c_str());

		rsd.cAltitudeLocal = std::atof(cPBoxAltitudeLocal->GetText().c_str());
		rsd.cTemperaturaLocal = std::atof(cPBoxTemperaturaLocal->GetText().c_str());
		
		rsd.cPFogueteData = &cFogueteData;
		if (cPSimulation)
		{
			delete cPSimulation;
		}
		cPSimulation = new RocketSim(rsd);

		switch (cPRadSimAte->GetMarkedButton())
		{
		case 0:
			cPSimulation->SimularAteChao(std::atof(cPBoxDeltaT->GetText().c_str()));
			cSimulado = true;
			break;
		case 1:
			cPSimulation->SimularSubida(std::atof(cPBoxDeltaT->GetText().c_str()));
			cSimulado = true;
			break;
		case 2:
			cPSimulation->Simular(std::atof(cPBoxTempoSim->GetText().c_str()), std::atof(cPBoxDeltaT->GetText().c_str()));
			cSimulado = true;
			break;
		}
	}

	if (cPBotExPos->ReturnLastState() && cSimulado)
	{
		std::string savePath = GetPathFromSaveFileA("CSV Documento Separado por Ponto e Virgula\0*.csv\0\0", "*.csv");

		if(savePath.size() != 0)
			cDataExporter.ExportLinearPositions(*cPSimulation->PegarDadosSim(), savePath);
	}

	if (cPBotExAccel->ReturnLastState() && cSimulado)
	{
		std::string savePath = GetPathFromSaveFileA("CSV Documento Separado por Ponto e Virgula\0*.csv\0\0", "*.csv");

		if (savePath.size() != 0)
			cDataExporter.ExportLinearAccelerations(*cPSimulation->PegarDadosSim(), savePath);
	}

	if (cPBotExVels->ReturnLastState() && cSimulado)
	{
		std::string savePath = GetPathFromSaveFileA("CSV Documento Separado por Ponto e Virgula\0*.csv\0\0", "*.csv");

		if (savePath.size() != 0)
			cDataExporter.ExportLinearVelocities(*cPSimulation->PegarDadosSim(), savePath);
	}

	if (cPBotExAtit->ReturnLastState() && cSimulado)
	{
		std::string savePath = GetPathFromSaveFileA("CSV Documento Separado por Ponto e Virgula\0*.csv\0\0", "*.csv");

		if (savePath.size() != 0)
			cDataExporter.ExportAtitudes(*cPSimulation->PegarDadosSim(), savePath);
	}

	if (cPBotExVelsAng->ReturnLastState() && cSimulado)
	{
		std::string savePath = GetPathFromSaveFileA("CSV Documento Separado por Ponto e Virgula\0*.csv\0\0", "*.csv");

		if (savePath.size() != 0)
			cDataExporter.ExportAngularVelocities(*cPSimulation->PegarDadosSim(), savePath);
	}

	if (cPBotExAccsAng->ReturnLastState() && cSimulado)
	{
		std::string savePath = GetPathFromSaveFileA("CSV Documento Separado por Ponto e Virgula\0*.csv\0\0", "*.csv");

		if (savePath.size() != 0)
			cDataExporter.ExportAngularVelocities(*cPSimulation->PegarDadosSim(), savePath);
	}

	/*cCamera3D.SetPosition(cZoom*sinf(cPosXY.x), cZoom *sinf(cPosXY.y),
		cZoom * cos(cPosXY.x));
	DirectX::XMVECTOR v = { cZoom*sinf(cPosXY.x), cZoom *sinf(cPosXY.y),
		cZoom * cos(cPosXY.x) };
	v = DirectX::XMVector3Normalize(v);
	cCamera3D.LookAt(-v.m128_f32[0], -v.m128_f32[1], -v.m128_f32[2]);*/

	cCamera3D.SetPosition(0.0f, -(float(cFogueteData.cAlturaFoguete) / 2.0f),
		cZoom);
	cCamera3D.LookAt(0.0f, 0.0f, 1.0f);
}

template<class T>
void RocketSimulator::Delete(T** p)
{
	if (*p)
	{
		delete *p;
		*p = nullptr;
	}
}

void RocketSimulator::SubMenuWeatherDraw()
{
	cBackgroundSubWeather.Draw(cPGraphics);

	cPBoxAltitudeLocal->Draw(cPosMouseAgora, cPGraphics);
	cPBoxTemperaturaLocal->Draw(cPosMouseAgora, cPGraphics);
}

void RocketSimulator::SubMenuInfoDraw()
{
	if (!cSimulado)
	{
		cPWhiteFontDrawer->DrawString("Sem simulacao.", { 1384.0f, 211.0f }, 24, cPGraphics);
	}
	else
	{
		std::string buf = "";

		buf = std::to_string(static_cast<double>(
			static_cast<int>(cPSimulation->PegarDadosSim()->cTempoTotal * 100.0) / 100.0));

		ClearZeros(&buf);

		buf = "Tempo Total: " + buf + " s";

		cPWhiteFontDrawer->DrawString(buf, { 1384.0f, 211.0f }, 24, cPGraphics);

		buf = std::to_string(static_cast<double>(
			static_cast<int>(0.0 * 100.0) / 100.0));

		ClearZeros(&buf);

		buf = "Tempo Apogeu: " + buf + " s";

		cPWhiteFontDrawer->DrawString(buf, { 1384.0f, 253.0f }, 24, cPGraphics);

		buf = std::to_string(static_cast<double>(
			static_cast<int>(cPSimulation->PegarDadosSim()->cAltitudeMax * 100.0) / 100.0));

		ClearZeros(&buf);

		buf = "Apogeu: " + buf + " m";

		cPWhiteFontDrawer->DrawString(buf, { 1384.0f, 291.0f }, 24, cPGraphics);

		buf = std::to_string(static_cast<double>(
			static_cast<int>(cPSimulation->PegarDadosSim()->cVelocidadeMax * 100.0) / 100.0));

		ClearZeros(&buf);

		buf = "Velo. Max.: " + buf + " m/s";

		cPWhiteFontDrawer->DrawString(buf, { 1384.0f, 328.0f }, 24, cPGraphics);

		buf = std::to_string(static_cast<double>(
			static_cast<int>(cPSimulation->PegarDadosSim()->cAceleracaoMax * 100.0) / 100.0));

		ClearZeros(&buf);

		buf = "Acel. Max.: " + buf + " m/s2";

		cPWhiteFontDrawer->DrawString(buf, { 1384.0f, 370.0f }, 24, cPGraphics);

		const VETOR posF = cPSimulation->PegarDadosSim()->cPosicoes.back();
		std::string x = std::to_string(static_cast<double>(
			static_cast<int>(posF.x* 100.0) / 100.0));
		std::string y = std::to_string(static_cast<double>(
			static_cast<int>(posF.y* 100.0) / 100.0));
		std::string z = std::to_string(static_cast<double>(
			static_cast<int>(posF.z* 100.0) / 100.0));

		ClearZeros(&x);
		ClearZeros(&y);
		ClearZeros(&z);

		buf = "("+x+", "+y+", "+z+") m";
		
		buf = "Pos. Final: " + buf;

		cPWhiteFontDrawer->DrawString(buf, { 1384.0f, 410.0f }, 24, cPGraphics);

		const double alphaMaxGraus = cPSimulation->PegarDadosSim()->cAlphaMaximo * 180.0 / 3.14159;

		buf = std::to_string(static_cast<double>(
			static_cast<int>(alphaMaxGraus * 10000.0) / 10000.0));

		ClearZeros(&buf);

		buf = "Alpha Max.: " + buf + " graus";

		cPWhiteFontDrawer->DrawString(buf, { 1384.0f, 446.0f }, 24, cPGraphics);
	}
}

void RocketSimulator::SubMenuSimDraw()
{
	cBackgroundSubDir.Draw(cPGraphics);

	cPBotSimular->Draw(cPosMouseAgora, cPGraphics);
	cPBotSalvar->Draw(cPosMouseAgora, cPGraphics);
	cPBotAbrir->Draw(cPosMouseAgora, cPGraphics);
	cPBotExPos->Draw(cPosMouseAgora, cPGraphics);
	cPBotExVels->Draw(cPosMouseAgora, cPGraphics);
	cPBotExAccel->Draw(cPosMouseAgora, cPGraphics);
	cPBotExAtit->Draw(cPosMouseAgora, cPGraphics);
	cPBotExVelsAng->Draw(cPosMouseAgora, cPGraphics);
	cPBotExAccsAng->Draw(cPosMouseAgora, cPGraphics);

	if (cSimulado)
	{
		//Apogeu
		std::string buf = std::to_string(int(cPSimulation->PegarDadosSim()->cAltitudeMax * 10.0) / 10.0);
		ClearZeros(&buf);
		buf += " m";
		cPWhiteFontDrawer->DrawString(buf, { 1593.0f, 322.0f }, 55.0f, cPGraphics);

		//AcelMax
		buf = std::to_string(int(cPSimulation->PegarDadosSim()->cAceleracaoMax * 10.0) / 10.0);
		ClearZeros(&buf);
		buf += " m/s2";
		cPWhiteFontDrawer->DrawString(buf, { 1593.0f, 397.0f }, 55.0f, cPGraphics);

		//VelMax
		buf = std::to_string(int(cPSimulation->PegarDadosSim()->cVelocidadeMax * 10.0) / 10.0);
		ClearZeros(&buf);
		buf += " m/s";
		cPWhiteFontDrawer->DrawString(buf, { 1593.0f, 472.0f }, 55.0f, cPGraphics);

		buf = std::to_string(int(cPSimulation->PegarDadosSim()->cVelocidadeMax * 10.0) / 10.0);
		ClearZeros(&buf);
		buf += " m/s";
		cPWhiteFontDrawer->DrawString(buf, { 1593.0f, 472.0f }, 55.0f, cPGraphics);
	}
}

void RocketSimulator::SubMenuWeatherUpdateRocket()
{

}
void RocketSimulator::SubMenuInfoUpdateRocket()
{

}
void RocketSimulator::SubMenuSimUpdateRocket()
{

}

void RocketSimulator::HandleSubWeather(TIPO t, const DirectX::XMFLOAT2& mousPos, char key)
{
	switch (t)
	{
	case MOUSEPRESS:
	{
		cPBoxAltitudeLocal->MousePress(mousPos, cPGraphics);
		cPBoxTemperaturaLocal->MousePress(mousPos, cPGraphics);
		break;
	}
	case MOUSERELEASE:
	{
		break;
	}
	case CHARPRESS:
	{
		cPBoxAltitudeLocal->KeyPress(key);
		cPBoxTemperaturaLocal->KeyPress(key);
		break;
	}
	}
}

void RocketSimulator::HandleSubInfo(TIPO t, const DirectX::XMFLOAT2& mousPos, char key)
{
	switch (t)
	{
	case MOUSEPRESS:
	{

		break;
	}
	case MOUSERELEASE:
	{
		break;
	}
	case CHARPRESS:
	{
		break;
	}
	}
}

void RocketSimulator::HandleSubSim(TIPO t, const DirectX::XMFLOAT2& mousePos, char key)
{
	switch (t)
	{
	case MOUSEPRESS:
	{
		cPBotSimular->MousePress(mousePos, cPGraphics);
		cPBotSalvar->MousePress(mousePos, cPGraphics);
		cPBotAbrir->MousePress(mousePos, cPGraphics);

		cPBotExPos->MousePress(mousePos, cPGraphics);
		cPBotExVels->MousePress(mousePos, cPGraphics);
		cPBotExAccel->MousePress(mousePos, cPGraphics);
		cPBotExAtit->MousePress(mousePos, cPGraphics);
		cPBotExVelsAng->MousePress(mousePos, cPGraphics);
		cPBotExAccsAng->MousePress(mousePos, cPGraphics);
		break;
	}
	case MOUSERELEASE:
	{
		cPBotSimular->MouseRelease(mousePos, cPGraphics);
		cPBotSalvar->MouseRelease(mousePos, cPGraphics);
		cPBotAbrir->MouseRelease(mousePos, cPGraphics);

		cPBotExPos->MouseRelease(mousePos, cPGraphics);
		cPBotExVels->MouseRelease(mousePos, cPGraphics);
		cPBotExAccel->MouseRelease(mousePos, cPGraphics);
		cPBotExAtit->MouseRelease(mousePos, cPGraphics);
		cPBotExVelsAng->MouseRelease(mousePos, cPGraphics);
		cPBotExAccsAng->MouseRelease(mousePos, cPGraphics);
		break;
	}
	case CHARPRESS:
	{
		break;
	}
	}
}