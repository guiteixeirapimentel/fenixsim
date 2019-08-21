#define _USE_MATH_DEFINES
#include "RocketBuilder.h"
#include "WinWrappers.h"
#include <cmath>
#include "Program.h"

RocketBuilder::RocketBuilder(KeyboardClient& kbd, MouseClient& mouse, GraphicsDxD* pGfx, MEN_ID* pMenId)
	:
	cPGraphics(pGfx),
	cKbd(kbd),
	cMouse(mouse),
	cPMenuId(pMenId),
	cPRocketModel(nullptr),
	cBackgroundTemplate(L"Assets/UI/RocketBuilder/background.png", { 1920.0f, 1080.0f }, { 0.0f, 0.0f }, DEFWNDDIMEN, cPGraphics),
	cBackgroundSubCoifa(L"Assets/UI/RocketBuilder/backgroundSubCoifa.png", { 460.0f, 674.0f }, { 71.0f, 193.0f }, DEFWNDDIMEN, cPGraphics),
	cBackgroundSubCorpo(L"Assets/UI/RocketBuilder/backgroundSubCorpo.png", { 302.0f, 46.0f }, { 245.0f, 180.0f }, DEFWNDDIMEN, cPGraphics),
	cBackgroundSubTubo(L"Assets/UI/RocketBuilder/backgroundSubCorpoTubo.png", { 488.0f, 369.0f }, { 68.0f, 456.0f }, DEFWNDDIMEN, cPGraphics),
	cBackgroundSubTransicao(L"Assets/UI/RocketBuilder/backgroundSubCorpoTransicao.png", { 498.0f, 499.0f }, { 63.0f, 456.0f }, DEFWNDDIMEN, cPGraphics),
	cBackgroundSubAletas(L"Assets/UI/RocketBuilder/backgroundSubAletas.png", { 493.0f, 727.0f }, { 70.0f, 184.0f }, DEFWNDDIMEN, cPGraphics),
	cBackgroundSubOpt(L"Assets/UI/RocketBuilder/backgroundSubOpt.png", { 216.0f, 322.0f }, { 1397.0f, 403.0f }, DEFWNDDIMEN, cPGraphics),
	cBackgroundSubParaq(L"Assets/UI/RocketBuilder/backgroundSubParaquedas.png", { 436.0f, 420.0f }, { 1395.0f, 181.0f }, DEFWNDDIMEN, cPGraphics),
	cPNomeFoguete(nullptr),
	cPZoomFitBut(nullptr),
	cPMenuEsq(nullptr),
	cPMenuDir(nullptr),
	cPBotMenuPrincipal(nullptr),
	cPBotFoguetes(nullptr),
	cPBotSimulacoes(nullptr),
	cPBotStudio(nullptr),
	cPButAbrirFoguete(nullptr),
	cPButReAbrirFoguete(nullptr),
	cPButSalvarFoguete(nullptr),
	cPButSalvarComoFoguete(nullptr),
	cPSelectorPartesCorpo(nullptr),
	cPosCPEsf(1.0f, 100, 100, L"Assets/UI/RocketBuilder/cpEsfText.png", cPGraphics),
	cPosCGEsf(1.0f, 100, 100, L"Assets/UI/RocketBuilder/cgEsfText.png", cPGraphics),
	cConsolasBranca1324(L"Assets/Fontes/Consolas13x24Branca.png", { 13.0f, 24.0f }, cPGraphics),
	cConsolasPreta1324(L"Assets/Fontes/Consolas13x24Preta.png", { 13.0f, 24.0f }, cPGraphics),
	cConsolasDrawerBranca1324(cConsolasBranca1324, DEFWNDDIMEN),
	cConsolasDrawerPreta1324(cConsolasPreta1324, DEFWNDDIMEN),
	cFlechaA(L"imgs/azul.png", pGfx),
	cFlechaV(L"imgs/vermelho.png", pGfx),
	cFlechaR(L"imgs/rosa.png", pGfx),
	cPNomeArqMotor(nullptr),
	cBackgroundSubMotor(L"Assets/UI/RocketBuilder/backgroundSubMotor.png", { 478.0f, 724.0f }, { 1384.0f, 197.0f }, DEFWNDDIMEN, cPGraphics),
	cArquivoMotorNaoEncontrado(false),
	cUltNomeMotor(""),
	cFogPathOpen(""),
	cZoom(-2.0f),
	cRot(0.0f)
{
	cPosXY.x = 0.0f;
	cPosXY.y = -5.0f;
	cCamera3D.SetPosition(cPosXY.x, cPosXY.y, cZoom);

	BoxDesc boxDesc = {};
	boxDesc.fileNameDefault = L"Assets/UI/RocketBuilder/nFogDef.png";
	boxDesc.fileNameSelected = L"Assets/UI/RocketBuilder/nFogSel.png";
	boxDesc.pFont = &cConsolasBranca1324;
	boxDesc.fontSize = { 36.0f, 67.0f };
	boxDesc.pos = { 649.0f, 6.0f };
	boxDesc.size = { 619.0f, 89.0f };
	boxDesc.defWndSize = DEFWNDDIMEN;
	boxDesc.centralizado = true;

	cPNomeFoguete = new TextBox(boxDesc, cPGraphics);
	cPNomeFoguete->SetText("Fenix 0");

	ButtonDesc butDesc = {};

	butDesc.fileNameDefault = L"Assets/UI/RocketBuilder/botMenPrinDef.png";
	butDesc.fileNamePressed = L"Assets/UI/RocketBuilder/botMenPrinPress.png";
	butDesc.filenameRollover = L"Assets/UI/RocketBuilder/botMenPrinRoll.png";

	butDesc.pos = { 63.0f, 992.0f };
	butDesc.size = { 442.0f, 70.0f };
	butDesc.defWndSize = boxDesc.defWndSize;

	cPBotMenuPrincipal = new PushButton(butDesc, cPGraphics);

	butDesc.pos = { 520.0f, 992.0f };

	butDesc.fileNameDefault = L"Assets/UI/RocketBuilder/botMenSimDef.png";
	butDesc.fileNamePressed = L"Assets/UI/RocketBuilder/botMenSimPress.png";
	butDesc.filenameRollover = L"Assets/UI/RocketBuilder/botMenSimRoll.png";

	cPBotSimulacoes = new PushButton(butDesc, cPGraphics);

	butDesc.pos = { 976.0f, 992.0f };

	butDesc.fileNameDefault = L"Assets/UI/RocketBuilder/botMenFogDef.png";
	butDesc.fileNamePressed = L"Assets/UI/RocketBuilder/botMenFogPress.png";
	butDesc.filenameRollover = L"Assets/UI/RocketBuilder/botMenFogRoll.png";

	cPBotFoguetes = new PushButton(butDesc, cPGraphics);

	butDesc.pos = { 1432.0f, 992.0f };

	butDesc.fileNameDefault = L"Assets/UI/RocketBuilder/botMenStuDef.png";
	butDesc.fileNamePressed = L"Assets/UI/RocketBuilder/botMenStuPress.png";
	butDesc.filenameRollover = L"Assets/UI/RocketBuilder/botMenStuRoll.png";

	cPBotStudio = new PushButton(butDesc, cPGraphics);

	cPMenuDir = new Menu();
	cPMenuEsq = new Menu();

	butDesc.pos = { 63.0f, 42.0f };
	butDesc.size = { 164.0f, 102.0f };

	butDesc.fileNameDefault = L"Assets/UI/RocketBuilder/botMenCoifDef.png";
	butDesc.fileNamePressed = L"Assets/UI/RocketBuilder/botMenCoifPress.png";
	butDesc.filenameRollover = L"Assets/UI/RocketBuilder/botMenCoifRoll.png";

	cPMenuEsq->AddButton(butDesc, L"Assets/UI/RocketBuilder/botMenCoifSwitch.png", cPGraphics);


	butDesc.pos = { 231.0f, 42.0f };

	butDesc.fileNameDefault = L"Assets/UI/RocketBuilder/botMenCorDef.png";
	butDesc.fileNamePressed = L"Assets/UI/RocketBuilder/botMenCorPress.png";
	butDesc.filenameRollover = L"Assets/UI/RocketBuilder/botMenCorRoll.png";

	cPMenuEsq->AddButton(butDesc, L"Assets/UI/RocketBuilder/botMenCorSwitch.png", cPGraphics);


	butDesc.pos = { 399.0f, 42.0f };

	butDesc.fileNameDefault = L"Assets/UI/RocketBuilder/botMenAleDef.png";
	butDesc.fileNamePressed = L"Assets/UI/RocketBuilder/botMenAlePress.png";
	butDesc.filenameRollover = L"Assets/UI/RocketBuilder/botMenAleRoll.png";

	cPMenuEsq->AddButton(butDesc, L"Assets/UI/RocketBuilder/botMenAleSwitch.png", cPGraphics);


	butDesc.pos = { 1374.0f, 42.0f };
	butDesc.size = { 163.0f, 101.0f };

	butDesc.fileNameDefault = L"Assets/UI/RocketBuilder/botMenMotDef.png";
	butDesc.fileNamePressed = L"Assets/UI/RocketBuilder/botMenMotPress.png";
	butDesc.filenameRollover = L"Assets/UI/RocketBuilder/botMenMotRoll.png";

	cPMenuDir->AddButton(butDesc, L"Assets/UI/RocketBuilder/botMenMotSwitch.png", cPGraphics);

	butDesc.pos = { 1542.0f, 42.0f };

	butDesc.fileNameDefault = L"Assets/UI/RocketBuilder/botMenParDef.png";
	butDesc.fileNamePressed = L"Assets/UI/RocketBuilder/botMenParPress.png";
	butDesc.filenameRollover = L"Assets/UI/RocketBuilder/botMenParRoll.png";

	cPMenuDir->AddButton(butDesc, L"Assets/UI/RocketBuilder/botMenParSwitch.png", cPGraphics);

	butDesc.pos = { 1711.0f, 42.0f };

	butDesc.fileNameDefault = L"Assets/UI/RocketBuilder/botMenOptDef.png";
	butDesc.fileNamePressed = L"Assets/UI/RocketBuilder/botMenOptPress.png";
	butDesc.filenameRollover = L"Assets/UI/RocketBuilder/botMenOptRoll.png";

	cPMenuDir->AddButton(butDesc, L"Assets/UI/RocketBuilder/botMenOptSwitch.png", cPGraphics);

	//Textbox menus
	boxDesc.fileNameDefault = L"Assets/UI/RocketBuilder/textboxdef.png";
	boxDesc.fileNameSelected = L"Assets/UI/RocketBuilder/textboxsel.png";

	boxDesc.pFont = &cConsolasPreta1324;
	boxDesc.fontSize = { 15.17f, 28.0f };
	boxDesc.size = { 140.0f, 52.0f };
	boxDesc.defWndSize = DEFWNDDIMEN;
	boxDesc.centralizado = false;


	//SUBMENU COIFA
	boxDesc.pos = { 330.0f, 474.0f };
	cPRaioInterCoif = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 330.0f, 553.0f };
	cPRaioExterCoif = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 330.0f, 637.0f };
	cPAlturaCoif = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 330.0f, 728.0f };
	cPDensidadeCoif = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 330.0f, 825.0f };
	cPRugosMediaCoif = new NumBox(boxDesc, cPGraphics);

	cPTipoCoifa = new RadioTuple();

	butDesc.fileNameDefault = L"Assets/UI/RocketBuilder/radioDef.png";
	butDesc.fileNamePressed = L"Assets/UI/RocketBuilder/radioDef.png";
	butDesc.filenameRollover = L"Assets/UI/RocketBuilder/radioDef.png";

	butDesc.pos = { 204.0f, 276.0f };
	butDesc.size = { 80.0f, 66.0f };

	cPTipoCoifa->AddButton(butDesc, L"Assets/UI/RocketBuilder/radioChecked.png", cPGraphics);

	butDesc.pos = { 375.0f, 280.0f };
	cPTipoCoifa->AddButton(butDesc, L"Assets/UI/RocketBuilder/radioChecked.png", cPGraphics);

	//Submenu corpo
	boxDesc.size = { 140.0f, 52.0f };

	/// TRANSICAO
	boxDesc.pos = { 348.0f, 523.0f };
	cPRaioIntTopoTrans = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 348.0f, 651.0f };
	cPRaioIntBaseTrans = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 348.0f, 588.0f };
	cPRaioExtTopoTrans = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 348.0f, 716.0f };
	cPRaioExtBaseTrans = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 348.0f, 781.0f };
	cPAlturaTrans = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 348.0f, 846.0f };
	cPDensidadeTrans = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 348.0f, 911.0f };
	cPRugosMediaTrans = new NumBox(boxDesc, cPGraphics);
	
	/// TUBO

	boxDesc.pos = { 322.0f, 529.0f };
	cPRaioIntTubo = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 322.0f, 594.0f };
	cPRaioExtTubo = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 322.0f, 657.0f };
	cPAlturaTubo = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 322.0f, 722.0f };
	cPDensidadeTubo = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 322.0f, 787.0f };
	cPRugosMediaTubo = new NumBox(boxDesc, cPGraphics);

	//Submenu Aletas

	boxDesc.pos = { 328.0f, 272.0f };
	cPNumAletas = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 328.0f, 343.0f };
	cPCordaRaizAle = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 328.0f, 412.0f };
	cPCordaPontaAle = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 328.0f, 484.0f };
	cPDeslocCordaPontaAle = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 328.0f, 555.0f };
	cPEnvergaduraAle = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 328.0f, 626.0f };
	cPPosVerticalAle = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 328.0f, 697.0f };
	cPEspessuraAle = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 328.0f, 767.0f };
	cPRugosidadeAle = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 328.0f, 838.0f };
	cPDensidadeAle = new NumBox(boxDesc, cPGraphics);

	//Submenu Paraquedas

	boxDesc.pos = { 1624.0f, 278.0f };
	cPBoxDiamParaq = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 1624.0f, 348.0f };
	cPBoxCDParaq = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 1624.0f, 418.0f };
	cPBoxPosVertParaq = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 1624.0f, 488.0f };
	cPBoxMassaParaq = new NumBox(boxDesc, cPGraphics);

	boxDesc.pos = { 1624.0f, 558.0f };
	cPBoxTempoEjecaoParaq = new NumBox(boxDesc, cPGraphics);

	//Submenu Motor
	boxDesc.pos = { 1399.0f, 426.0f };
	boxDesc.size = { 380.0f, 52.0f };
	boxDesc.fileNameDefault = L"Assets/UI/RocketBuilder/textboxlargdef.png";
	cPNomeArqMotor = new TextBox(boxDesc, cPGraphics);

	butDesc.fileNameDefault = L"Assets/UI/RocketBuilder/botBuscDef.png";
	butDesc.fileNamePressed = L"Assets/UI/RocketBuilder/botBuscPress.png";
	butDesc.filenameRollover = L"Assets/UI/RocketBuilder/botBuscRoll.png";

	butDesc.pos = { 1793.0f, 426.0f };
	butDesc.size = { 55.0f, 51.0f };

	cPSearchMotorFileBut = new EventButton(butDesc, cPGraphics);

	butDesc.fileNameDefault = L"Assets/UI/RocketBuilder/openDef.png";
	butDesc.fileNamePressed = L"Assets/UI/RocketBuilder/openPress.png";
	butDesc.filenameRollover = L"Assets/UI/RocketBuilder/openRoll.png";

	butDesc.pos = { 1419.0f, 192.0f };
	butDesc.size = { 149.0f, 65.0f };

	cPButAbrirFoguete = new EventButton(butDesc, cPGraphics);

	butDesc.fileNameDefault = L"Assets/UI/RocketBuilder/reOpenDef.png";
	butDesc.fileNamePressed = L"Assets/UI/RocketBuilder/reOpenPress.png";
	butDesc.filenameRollover = L"Assets/UI/RocketBuilder/reOpenRoll.png";

	butDesc.pos = { 1592.0f, 192.0f };
	butDesc.size = { 231.0f, 65.0f };

	cPButReAbrirFoguete = new EventButton(butDesc, cPGraphics);

	butDesc.fileNameDefault = L"Assets/UI/RocketBuilder/saveDef.png";
	butDesc.fileNamePressed = L"Assets/UI/RocketBuilder/savePress.png";
	butDesc.filenameRollover = L"Assets/UI/RocketBuilder/saveRoll.png";

	butDesc.pos = { 1419.0f, 275.0f };
	butDesc.size = { 149.0f, 65.0f };

	cPButSalvarFoguete = new EventButton(butDesc, cPGraphics);

	butDesc.fileNameDefault = L"Assets/UI/RocketBuilder/saveAsDef.png";
	butDesc.fileNamePressed = L"Assets/UI/RocketBuilder/saveAsPress.png";
	butDesc.filenameRollover = L"Assets/UI/RocketBuilder/saveAsRoll.png";

	butDesc.pos = { 1592.0f, 275.0f };
	butDesc.size = { 231.0f, 65.0f };

	cPButSalvarComoFoguete = new EventButton(butDesc, cPGraphics);

	butDesc.fileNameDefault = L"Assets/UI/RocketBuilder/plusButtonDef.png";
	butDesc.fileNamePressed = L"Assets/UI/RocketBuilder/plusButtonPress.png";
	butDesc.filenameRollover = L"Assets/UI/RocketBuilder/plusButtonRoll.png";

	butDesc.pos = { 386.0f, 202.0f };
	butDesc.size = { 52.0f, 44.0f };

	cPButAddTubo = new EventButton(butDesc, cPGraphics);
	
	butDesc.pos = { 446.0f, 202.0f };
	butDesc.size = { 52.0f, 44.0f };

	cPButAddTran = new EventButton(butDesc, cPGraphics);

	butDesc.fileNameDefault = L"Assets/UI/RocketBuilder/minusButtonDef.png";
	butDesc.fileNamePressed = L"Assets/UI/RocketBuilder/minusButtonPress.png";
	butDesc.filenameRollover = L"Assets/UI/RocketBuilder/minusButtonRoll.png";

	butDesc.pos = { 505.0f, 202.0f };
	butDesc.size = { 52.0f, 44.0f };

	cPButDelPart = new EventButton(butDesc, cPGraphics);
	
	ButtonDesc upArrow = butDesc;
	upArrow.fileNameDefault = L"Assets/UI/RocketBuilder/selector/upArrowSelectorDef.png";
	upArrow.fileNamePressed = L"Assets/UI/RocketBuilder/selector/upArrowSelectorPress.png";
	upArrow.filenameRollover = L"Assets/UI/RocketBuilder/selector/upArrowSelectorRoll.png";
	upArrow.pos = { 488.0f, 269.0f };
	upArrow.size = { 59.0f, 60.0f };

	ButtonDesc downArrow = butDesc;
	downArrow.fileNameDefault = L"Assets/UI/RocketBuilder/selector/downArrowSelectorDef.png";
	downArrow.fileNamePressed = L"Assets/UI/RocketBuilder/selector/downArrowSelectorPress.png";
	downArrow.filenameRollover = L"Assets/UI/RocketBuilder/selector/downArrowSelectorRoll.png";
	downArrow.pos = { 488.0f, 336.0f };
	downArrow.size = { 59.0f, 60.0f };	

	cPSelectorPartesCorpo = new ScrollerSelector({ 400.0f, 135.0f }, { 77.0f, 264.0f }, { 13.0f, 24.0f }, 
	{ 5.0f, 5.0f }, 3.0f,
		L"Assets/UI/RocketBuilder/selector/selecterBackground.png", downArrow, upArrow, cConsolasBranca1324, cConsolasPreta1324, 
		butDesc.defWndSize, cPGraphics);

	cPSelectorPartesCorpo->AddOption("0. Coifa Conica");

	cPRocketModel = new RocketModelBuilder(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, cPGraphics);		

	cFogueteData.CriarAletaTrapezoidal(0.0, 0.0, 0.0, 0, Aleta::ARREDONDADO, 0.0, 0.0, 0.0, 0.0, 0.0);
	cFogueteData.CriarCoifaConica(0.0, 0.0, 0.0, 0.0, 0.0);
}

RocketBuilder::~RocketBuilder()
{
	Delete(&cPRocketModel);
	Delete(&cPNomeFoguete);
	Delete(&cPZoomFitBut);
	Delete(&cPMenuEsq);
	Delete(&cPMenuDir);

	Delete(&cPBotFoguetes);
	Delete(&cPBotMenuPrincipal);
	Delete(&cPBotSimulacoes);
	Delete(&cPBotStudio);

	Delete(&cPRaioInterCoif);
	Delete(&cPRaioExterCoif);
	Delete(&cPAlturaCoif);
	Delete(&cPDensidadeCoif);
	Delete(&cPRugosMediaCoif);
	Delete(&cPTipoCoifa);

	Delete(&cPBoxDiamParaq);
	Delete(&cPBoxCDParaq);
	Delete(&cPBoxPosVertParaq);
	Delete(&cPBoxMassaParaq);
	Delete(&cPBoxTempoEjecaoParaq);

	Delete(&cPSelectorPartesCorpo);
	Delete(&cPRaioIntTopoTrans);
	Delete(&cPRaioIntBaseTrans);
	Delete(&cPRaioExtTopoTrans);
	Delete(&cPRaioExtBaseTrans);
	Delete(&cPAlturaTrans);
	Delete(&cPDensidadeTrans);
	Delete(&cPRugosMediaTrans);

	Delete(&cPButAddTubo);
	Delete(&cPButAddTran);
	Delete(&cPButDelPart);
		   
	Delete(&cPRaioIntTubo);
	Delete(&cPRaioExtTubo);
	Delete(&cPAlturaTubo);
	Delete(&cPDensidadeTubo);
	Delete(&cPRugosMediaTubo);
		
	Delete(&cPNumAletas);
	Delete(&cPCordaRaizAle);
	Delete(&cPCordaPontaAle);
	Delete(&cPDeslocCordaPontaAle);
	Delete(&cPEnvergaduraAle);
	Delete(&cPPosVerticalAle);
	Delete(&cPEspessuraAle);
	Delete(&cPRugosidadeAle);
	Delete(&cPDensidadeAle);

	Delete(&cPNomeArqMotor);
	Delete(&cPSearchMotorFileBut);

	Delete(&cPButAbrirFoguete);
	Delete(&cPButSalvarFoguete);
	Delete(&cPButReAbrirFoguete);
	Delete(&cPButSalvarComoFoguete);
}

void RocketBuilder::Tick()
{
	cPGraphics->IniciarCena();
	Draw();
	cPGraphics->TerminarCena();

	Update();
}

void RocketBuilder::Draw()
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

	const unsigned int numFins = std::atoi(cPNumAletas->GetText().c_str());
	const float alturaCorpo = (float)cFogueteData.cAlturaFoguete;
	const float alturaCoifa = (float)std::atof(cPAlturaCoif->GetText().c_str()) / 100.0f;
	const float raioCorpo = (float)cFogueteData.cRaioMaximo;
	const float raioBaseCoifa = (float)std::atof(cPRaioExterCoif->GetText().c_str()) / 100.0f;
	const float deslAletBaseCorpo = (float)std::atof(cPPosVerticalAle->GetText().c_str()) / 100.f;
	const float rot = cRot;

	const DirectX::XMFLOAT3 posCp = { -2.0f * raioCorpo,
		(float)(-cFogueteData.cPosVCPFoguete), 0.0f };

	const DirectX::XMFLOAT3 posCg = { 2.0f * raioCorpo,
		(float)(-cFogueteData.cPosVCGFoguete), 0.0f };
	static float rrr = 0.0f;
	rrr += 0.01f;
	cPosCPEsf.Render(posCp, { 0.0f, rrr, 0.0f }, raioCorpo / 1.8f, viewMatrix3D, cPGraphics);
	cPosCGEsf.Render(posCg, { 0.0f, rrr, 0.0f }, raioCorpo / 1.8f, viewMatrix3D, cPGraphics);
	
	cPRocketModel->Raster(cFogueteData, 0.0f, rot, { 0.0, 1.0, 0.0 }, viewMatrix3D, cPGraphics);

	/*if(cRockData.cData.cCPXIniFoguete != 0.0 && raioCorpo != 0.0)
		cFlechaA.Render({ double(-3.5f * raioCorpo), double((cRockData.GetData().cAlturaCorpo / 2.0f) + cRockData.GetData().cAlturaCoifa), 0.0 },
		{ 0.0, -cRockData.cData.cCPXIniFoguete, 0.0 },
			viewMatrix3D, cPGraphics);*/

	const VETOR posCoord = { 1.5 * raioCorpo,
		0.0, 0.0 };

	cFlechaR.Render(posCoord, { -alturaCorpo / 5.0, 0.0, 0.0 }, viewMatrix3D, cPGraphics);
	cFlechaR.Render(posCoord, { 0.0, -alturaCorpo / 5.0, 0.0 }, viewMatrix3D, cPGraphics);
	cFlechaR.Render(posCoord, { 0.0, 0.0, -alturaCorpo / 5.0 }, viewMatrix3D, cPGraphics);

	cPGraphics->Enable2D();

	cBackgroundTemplate.Draw(cPGraphics);
	
	cPNomeFoguete->Draw(cPosMouseAgora, cPGraphics);
	
	cPBotFoguetes->Draw(cPosMouseAgora, cPGraphics);
	cPBotMenuPrincipal->Draw(cPosMouseAgora, cPGraphics);
	cPBotSimulacoes->Draw(cPosMouseAgora, cPGraphics);
	cPBotStudio->Draw(cPosMouseAgora, cPGraphics);
	
	cPMenuEsq->Draw(cPosMouseAgora, cPGraphics);
	cPMenuDir->Draw(cPosMouseAgora, cPGraphics);

	switch (cPMenuEsq->GetActiveButton())
	{
	case 0:
		SubMenuCoifaDraw();
		break;
	case 1:
		SubMenuCorpoDraw();
		break;
	case 2:
		SubMenuAletaDraw();
		break;
	}

	switch (cPMenuDir->GetActiveButton())
	{
	case 0:
		SubMenuMotorDraw();
		break;
	case 1:
		SubMenuParaqueDraw();
		break;
	case 2:
		SubMenuOpcoesDraw();
		break;
	}
}

void RocketBuilder::Update()
{
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

	cPosMouseAgora = { static_cast<float>(cMouse.GetMouseX()), static_cast<float>(cMouse.GetMouseY()) };

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

		if(cPosXY.x +  deltaX < 3.14159f / 4.0f && cPosXY.x + deltaX > -3.14159f / 4.0f)
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

	const float cordaRaiz = (float)std::atof(cPCordaRaizAle->GetText().c_str()) / 100.0f;
	const float cordaPonta = (float)std::atof(cPCordaPontaAle->GetText().c_str()) / 100.0f;
	const float deslCorda = (float)std::atof(cPDeslocCordaPontaAle->GetText().c_str()) / 100.0f;
	const float envergadura = (float)std::atof(cPEnvergaduraAle->GetText().c_str()) / 100.0f;
	const float espessura = (float)std::atof(cPEspessuraAle->GetText().c_str()) / 100.0f;
	cPRocketModel->SetAletaInfo(cordaRaiz, cordaPonta, deslCorda, envergadura, espessura, cPGraphics);

	while (cKbd.PeekChar())
	{
		unsigned char k = cKbd.ReadChar();

		cPNomeFoguete->KeyPress(k);

		switch (cPMenuEsq->GetActiveButton())
		{
		case 0:
			HandleSubCoifa(CHARPRESS, cPosMouseAgora, k);
			break;
		case 1:
			HandleSubCorpo(CHARPRESS, cPosMouseAgora, k);
			break;
		case 2:
			HandleSubAleta(CHARPRESS, cPosMouseAgora, k);
			break;
		}

		switch (cPMenuDir->GetActiveButton())
		{
		case 0:
			HandleSubMotor(CHARPRESS, cPosMouseAgora, k);
			break;
		case 1:
			HandleSubParaq(CHARPRESS, cPosMouseAgora, k);
			break;
		case 2:
			HandleSubOptio(CHARPRESS, cPosMouseAgora, k);
			break;
		}

	}

	auto me = cMouse.ReadMouse();
	if (me.IsValid())
	{
		if (me.GetType() == MouseEvent::LPress)
		{
			DirectX::XMFLOAT2 mousePos = { (float)me.GetX(), (float)me.GetY() };
			cPNomeFoguete->MousePress(mousePos, cPGraphics);

			cPBotFoguetes->MousePress(mousePos, cPGraphics);
			cPBotMenuPrincipal->MousePress(mousePos, cPGraphics);
			cPBotSimulacoes->MousePress(mousePos, cPGraphics);
			cPBotStudio->MousePress(mousePos, cPGraphics);
			cPMenuEsq->MousePress(mousePos, cPGraphics);
			cPMenuDir->MousePress(mousePos, cPGraphics);

			switch (cPMenuEsq->GetActiveButton())
			{
			case 0:
				HandleSubCoifa(MOUSEPRESS, mousePos, 0);
				break;
			case 1:
				HandleSubCorpo(MOUSEPRESS, mousePos, 0);
				break;
			case 2:
				HandleSubAleta(MOUSEPRESS, mousePos, 0);
				break;
			}

			switch (cPMenuDir->GetActiveButton())
			{
			case 0:
				HandleSubMotor(MOUSEPRESS, mousePos, 0);
				break;
			case 1:
				HandleSubParaq(MOUSEPRESS, mousePos, 0);
				break;
			case 2:
				HandleSubOptio(MOUSEPRESS, mousePos, 0);
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
			cPMenuEsq->MouseRelease(mousePos, cPGraphics);
			cPMenuDir->MouseRelease(mousePos, cPGraphics);

			switch (cPMenuEsq->GetActiveButton())
			{
			case 0:
				HandleSubCoifa(MOUSERELEASE, mousePos, 0);
				break;
			case 1:
				HandleSubCorpo(MOUSERELEASE, mousePos, 0);
				break;
			case 2:
				HandleSubAleta(MOUSERELEASE, mousePos, 0);
				break;
			}

			switch (cPMenuDir->GetActiveButton())
			{
			case 0:
				HandleSubMotor(MOUSERELEASE, mousePos, 0);
				break;
			case 1:
				HandleSubParaq(MOUSERELEASE, mousePos, 0);
				break;
			case 2:
				HandleSubOptio(MOUSERELEASE, mousePos, 0);
				break;
			}
		}
		else if (me.GetType() == MouseEvent::WheelUp)
		{
			float d = (0.05f * float(cFogueteData.cAlturaFoguete));
			if (cZoom + d < 0.0f)
				cZoom += d;
		}
		else if (me.GetType() == MouseEvent::WheelDown)
		{
			float d = (0.05f * float(cFogueteData.cAlturaFoguete));
			cZoom -= d;
		}
	}

	if (cUltNomeMotor.compare(cPNomeArqMotor->GetText()) != 0 &&
		cPNomeArqMotor->GetText().size() > 0)
	{
		cUltNomeMotor = cPNomeArqMotor->GetText();

		std::string nomeArqMot;// = "Motores/";
		nomeArqMot += cPNomeArqMotor->GetText();

		std::ifstream motor;
		motor.open(nomeArqMot.c_str());

		if (!motor.is_open())
		{
			cArquivoMotorNaoEncontrado = true;
		}
		else
		{
			cFogueteData.DefinirMotor(nomeArqMot, cFogueteData.cAlturaFoguete);
			cArquivoMotorNaoEncontrado = false;
		}
	}

	if (cPSearchMotorFileBut->ReturnLastState())
	{
		cPNomeArqMotor->SetText(GetPathFromOpenFileA("ENG\0*.eng\0\0"));
	}

	if (cPButAbrirFoguete->ReturnLastState())
	{
		std::string openPath = GetPathFromOpenFileA("FENIXR\0*.fog\0\0");
		if (openPath.size() != 0)
		{
			cFogPathOpen = openPath;
			OpenFile(openPath);
		}
	}

	if (cPButReAbrirFoguete->ReturnLastState() && cFogPathOpen.size() > 0)
	{
		OpenFile(cFogPathOpen);
	}

	if (cPButSalvarComoFoguete->ReturnLastState())
	{
		std::string savePath = GetPathFromSaveFileA("FENIXR\0*.fog\0\0", "*.fog");
		if (savePath.size() != 0)
		{
			cFogPathOpen = savePath;
			SaveFile(savePath);
		}
	}

	if (cPButSalvarFoguete->ReturnLastState() && cFogPathOpen.size() > 0)
	{
		SaveFile(cFogPathOpen);
	}

	if (cPButAddTran->ReturnLastState())
	{
		cFogueteData.CriarTransicao(0.1, 0.1, 0.1, 0.1, 0.1, 0.1, 0.1);

		cPSelectorPartesCorpo->AddOption(std::to_string(cFogueteData.cPartesCorpo.size() - 1) +". Transicao");

		cPRocketModel->AdicionarTransicao(0.1f, 0.1f, 0.1f, cFogueteData.cPartesCorpo.size() - 1, cPGraphics);
	}

	if (cPButAddTubo->ReturnLastState())
	{
		cFogueteData.CriarTubo(0.1, 0.1, 0.1, 0.1, 0.1);

		cPSelectorPartesCorpo->AddOption(std::to_string(cFogueteData.cPartesCorpo.size() - 1) + ". Tubo");
	}

	if (cPButDelPart->ReturnLastState() && cPSelectorPartesCorpo->GetSelectedOptionID() > 0)
	{
		auto selectedId = cPSelectorPartesCorpo->GetSelectedOptionID();

		if (cFogueteData.cTipoPartes[selectedId] == Foguete::TRANSICAOAUMENTO ||
			cFogueteData.cTipoPartes[selectedId] == Foguete::TRANSICAODIMINUI)
		{
			cPRocketModel->DeletarTransicao(selectedId);
		}

		if (!cPSelectorPartesCorpo->DeleteOption(selectedId) ||
			!cFogueteData.DeletarParteCorpo(selectedId))
		{
			MessageBox(nullptr, L"Erro ao excluir parte!", L"Exclusão", 0);
		}
	}

	//DirectX::XMVECTOR v = { cZoom*sinf(cPosXY.x), cZoom *sinf(cPosXY.y),
	//	cZoom * cos(cPosXY.x) };
	//v = DirectX::XMVector3Normalize(v);
	cCamera3D.SetPosition(0.0f, -(float(cFogueteData.cAlturaFoguete) / 2.0f),
		cZoom);
	cCamera3D.LookAt(0.0f, 0.0f, 1.0f);


	UpdateRocketData();
}

void RocketBuilder::SubMenuCoifaDraw()
{
	cBackgroundSubCoifa.Draw(cPGraphics);

	cPRaioInterCoif->Draw(cPosMouseAgora, cPGraphics);
	cPRaioExterCoif->Draw(cPosMouseAgora, cPGraphics);
	cPAlturaCoif->Draw(cPosMouseAgora, cPGraphics);
	cPDensidadeCoif->Draw(cPosMouseAgora, cPGraphics);
	cPRugosMediaCoif->Draw(cPosMouseAgora, cPGraphics);
	cPTipoCoifa->Draw(cPosMouseAgora, cPGraphics);
}
void RocketBuilder::SubMenuCorpoDraw()
{
	cBackgroundSubCorpo.Draw(cPGraphics);
	cPSelectorPartesCorpo->Draw(cPosMouseAgora, cPGraphics);

	cPButAddTubo->Draw(cPosMouseAgora, cPGraphics);
	cPButAddTran->Draw(cPosMouseAgora, cPGraphics);
	cPButDelPart->Draw(cPosMouseAgora, cPGraphics);

	if (cFogueteData.cTipoPartes[cPSelectorPartesCorpo->GetSelectedOptionID()] == Foguete::TUBO)
	{
		cBackgroundSubTubo.Draw(cPGraphics);
		cConsolasDrawerBranca1324.DrawString(std::to_string(cPSelectorPartesCorpo->GetSelectedOptionID()),
		{ 338.0f, 450.0f }, 48.0f, cPGraphics);
		
		cPRaioIntTubo->Draw(cPosMouseAgora, cPGraphics);
		cPRaioExtTubo->Draw(cPosMouseAgora, cPGraphics);
		cPAlturaTubo->Draw(cPosMouseAgora, cPGraphics);
		cPDensidadeTubo->Draw(cPosMouseAgora, cPGraphics);
		cPRugosMediaTubo->Draw(cPosMouseAgora, cPGraphics);
	}
	else if (cFogueteData.cTipoPartes[cPSelectorPartesCorpo->GetSelectedOptionID()] == Foguete::TRANSICAOAUMENTO ||
		cFogueteData.cTipoPartes[cPSelectorPartesCorpo->GetSelectedOptionID()] == Foguete::TRANSICAODIMINUI)
	{
		cBackgroundSubTransicao.Draw(cPGraphics);

		cConsolasDrawerBranca1324.DrawString(std::to_string(cPSelectorPartesCorpo->GetSelectedOptionID()),
		{ 400.0f, 450.0f }, 48.0f, cPGraphics);

		cPRaioIntTopoTrans->Draw(cPosMouseAgora, cPGraphics);
		cPRaioIntBaseTrans->Draw(cPosMouseAgora, cPGraphics);
		cPRaioExtTopoTrans->Draw(cPosMouseAgora, cPGraphics);
		cPRaioExtBaseTrans->Draw(cPosMouseAgora, cPGraphics);
		cPAlturaTrans->Draw(cPosMouseAgora, cPGraphics);
		cPDensidadeTrans->Draw(cPosMouseAgora, cPGraphics);
		cPRugosMediaTrans->Draw(cPosMouseAgora, cPGraphics);
	}
}
void RocketBuilder::SubMenuAletaDraw()
{
	cBackgroundSubAletas.Draw(cPGraphics);
	cPNumAletas->Draw(cPosMouseAgora, cPGraphics);
	cPCordaRaizAle->Draw(cPosMouseAgora, cPGraphics);
	cPCordaPontaAle->Draw(cPosMouseAgora, cPGraphics);
	cPDeslocCordaPontaAle->Draw(cPosMouseAgora, cPGraphics);
	cPEnvergaduraAle->Draw(cPosMouseAgora, cPGraphics);
	cPPosVerticalAle->Draw(cPosMouseAgora, cPGraphics);
	cPEspessuraAle->Draw(cPosMouseAgora, cPGraphics);
	cPRugosidadeAle->Draw(cPosMouseAgora, cPGraphics);
	cPDensidadeAle->Draw(cPosMouseAgora, cPGraphics);
}
	 
void RocketBuilder::SubMenuMotorDraw()
{
	cBackgroundSubMotor.Draw(cPGraphics);
	cPNomeArqMotor->Draw(cPosMouseAgora, cPGraphics);
	cPSearchMotorFileBut->Draw(cPosMouseAgora, cPGraphics);

	const DirectX::XMFLOAT2 defWndSize = { 1920.0f / cPGraphics->GetWndWidth(),
		1080.0f / cPGraphics->GetWndHeight() };
	const DirectX::XMFLOAT2 offset = { cPGraphics->GetWndWidth() / -2.0f, cPGraphics->GetWndHeight() / -2.0f };

	if (cFogueteData.cPMotor != nullptr)
	{
		if (cFogueteData.cPMotor->Valido())
		{
			cConsolasBranca1324.Write(cFogueteData.cPMotor->PegNome(),
			{ (1623.0f / defWndSize.x) + offset.x, (594.f / defWndSize.y) + offset.y },
			{ 16.25f / defWndSize.x, 30.0f / defWndSize.y }, Font::CENTER, cPGraphics);

			cConsolasBranca1324.Write(std::to_string(cFogueteData.cPMotor->PegTempoQueima()) + " seg",
			{ (1650.0f / defWndSize.x) + offset.x, (676.f / defWndSize.y) + offset.y },
			{ 16.25f / defWndSize.x, 30.0f / defWndSize.y}, Font::LEFT, cPGraphics);

			cConsolasBranca1324.Write(std::to_string(cFogueteData.cPMotor->PegMassa(0.0)) + " kg",
			{ (1650.0f / defWndSize.x) + offset.x, (740.f / defWndSize.y) + offset.y },
			{ 16.25f / defWndSize.x, 30.0f / defWndSize.y }, Font::LEFT, cPGraphics);

			cConsolasBranca1324.Write(std::to_string(cFogueteData.cPMotor->PegDiametro()) + " m",
			{ (1650.0f / defWndSize.x) + offset.x, (812.f / defWndSize.y) + offset.y },
			{ 16.25f / defWndSize.x, 30.0f / defWndSize.y }, Font::LEFT, cPGraphics);

			cConsolasBranca1324.Write(std::to_string(cFogueteData.cPMotor->PegComprimento()) + " m",
			{ (1650.0f / defWndSize.x) + offset.x, (887.f / defWndSize.y) + offset.y },
			{ 16.25f / defWndSize.x, 30.0f / defWndSize.y }, Font::LEFT, cPGraphics);
		}
		else if (cArquivoMotorNaoEncontrado)
		{
			cConsolasBranca1324.Write("Erro ao abrir arquivo!", 
			{ (1383.0f / defWndSize.x) + offset.x, (500.f / defWndSize.y) + offset.y },
			{ 16.25f / defWndSize.x, 30.0f / defWndSize.y }, Font::LEFT, cPGraphics);
		}
		else
		{
			cConsolasBranca1324.Write("Erro! Favor verificar arquivo", 
			{ (1383.0f / defWndSize.x) + offset.x, (500.f / defWndSize.y) + offset.y },
			{ 16.25f / defWndSize.x, 30.0f / defWndSize.y }, Font::LEFT, cPGraphics);
			cConsolasBranca1324.Write("e tentar novamente.", 
			{ (1383.0f / defWndSize.x) + offset.x, (538.f / defWndSize.y) + offset.y },
			{ 16.25f / defWndSize.x, 30.0f / defWndSize.y }, Font::LEFT, cPGraphics);
		}
	}
	else if (cArquivoMotorNaoEncontrado)
	{
		cConsolasBranca1324.Write("Erro ao abrir arquivo!", 
		{ (1383.0f / defWndSize.x) + offset.x, (500.f / defWndSize.y) + offset.y },
		{ 16.25f / defWndSize.x, 30.0f / defWndSize.y }, Font::LEFT, cPGraphics);
	}
}
void RocketBuilder::SubMenuParaqueDraw()
{
	cBackgroundSubParaq.Draw(cPGraphics);

	cPBoxDiamParaq->Draw(cPosMouseAgora, cPGraphics);
	cPBoxCDParaq->Draw(cPosMouseAgora, cPGraphics);
	cPBoxPosVertParaq->Draw(cPosMouseAgora, cPGraphics);
	cPBoxMassaParaq->Draw(cPosMouseAgora, cPGraphics);
	cPBoxTempoEjecaoParaq->Draw(cPosMouseAgora, cPGraphics);
}
void RocketBuilder::SubMenuOpcoesDraw()
{
	cBackgroundSubOpt.Draw(cPGraphics);

	cPButSalvarFoguete->Draw(cPosMouseAgora, cPGraphics);
	cPButAbrirFoguete->Draw(cPosMouseAgora, cPGraphics);

	cPButSalvarComoFoguete->Draw(cPosMouseAgora, cPGraphics);
	cPButReAbrirFoguete->Draw(cPosMouseAgora, cPGraphics);

	double massaMotor = 0.0;
	if (cFogueteData.cPMotor != nullptr)
	{
		massaMotor = cFogueteData.cPMotor->PegMassa(0.0);
	}

	const double massaTotal = static_cast<int>((cFogueteData.cMassaFogueteComMotor) * 1000.0) / 1000.0;

	const DirectX::XMFLOAT2 defWndSize = { 1920.0f / cPGraphics->GetWndWidth(),
		1080.0f / cPGraphics->GetWndHeight() };
	const DirectX::XMFLOAT2 offset = { cPGraphics->GetWndWidth() / -2.0f, cPGraphics->GetWndHeight() / -2.0f };

	const double cp3Dig = static_cast<int>(cFogueteData.cPosVCPFoguete*10.0 * 100.0f) / 10.0;

	const double cm3Dig = static_cast<int>(cFogueteData.cPosVCGFoguete *10.0 * 100.0f) / 10.0;

	std::string textCP = std::to_string(cp3Dig);
	ClearZeros(&textCP);
	textCP += " cm";
	std::string textCG = std::to_string(cm3Dig);
	ClearZeros(&textCG);
	textCG += " cm";
	std::string textMa = std::to_string(massaTotal);
	ClearZeros(&textMa);
	textMa += " kg";
	std::string textAlt = std::to_string(0.0);
	ClearZeros(&textAlt);
	textAlt += " m";
	
	cConsolasBranca1324.Write(textCP,
	{ (1656.0f / defWndSize.x) + offset.x, (394.f / defWndSize.y) + offset.y },
	{ 24.92f / defWndSize.x, 46.0f / defWndSize.y }, Font::LEFT, cPGraphics);

	cConsolasBranca1324.Write(textCG,
	{ (1656.0f / defWndSize.x) + offset.x, (491.f / defWndSize.y) + offset.y },
	{ 24.92f / defWndSize.x, 46.0f / defWndSize.y }, Font::LEFT, cPGraphics);

	cConsolasBranca1324.Write(textMa,
	{ (1656.0f / defWndSize.x) + offset.x, (588.f / defWndSize.y) + offset.y },
	{ 24.92f / defWndSize.x, 46.0f / defWndSize.y }, Font::LEFT, cPGraphics);

	cConsolasBranca1324.Write(textAlt,
	{ (1656.0f / defWndSize.x) + offset.x, (685.f / defWndSize.y) + offset.y },
	{ 24.92f / defWndSize.x, 46.0f / defWndSize.y }, Font::LEFT, cPGraphics);
}
	 
void RocketBuilder::HandleSubCoifa(TIPO t, const DirectX::XMFLOAT2& mousPos, char key)
{
	switch (t)
	{
	case MOUSEPRESS:
		{
			cPRaioInterCoif->MousePress(mousPos, cPGraphics);
			cPRaioExterCoif->MousePress(mousPos, cPGraphics);
			cPAlturaCoif->MousePress(mousPos, cPGraphics);
			cPDensidadeCoif->MousePress(mousPos, cPGraphics);
			cPRugosMediaCoif->MousePress(mousPos, cPGraphics);
			cPTipoCoifa->MousePress(mousPos, cPGraphics);

			break;
		}		
	case MOUSERELEASE:
		{
			cPTipoCoifa->MouseRelease(mousPos, cPGraphics);
			break;
		}
	case CHARPRESS:
		{
			cPRaioInterCoif->KeyPress(key);
			cPRaioExterCoif->KeyPress(key);
			cPAlturaCoif->KeyPress(key);
			cPDensidadeCoif->KeyPress(key);
			cPRugosMediaCoif->KeyPress(key);
			break;
		}
	}
}
void RocketBuilder::HandleSubCorpo(TIPO t, const DirectX::XMFLOAT2& mousPos, char key)
{
	switch (t)
	{
	case MOUSEPRESS:
		{
			cPSelectorPartesCorpo->MousePress(mousPos, cPGraphics);
			cPButAddTubo->MousePress(mousPos, cPGraphics);
			cPButAddTran->MousePress(mousPos, cPGraphics);
			cPButDelPart->MousePress(mousPos, cPGraphics);

			if (cFogueteData.cTipoPartes[cPSelectorPartesCorpo->GetSelectedOptionID()] == Foguete::TUBO)
			{
				cPRaioIntTubo->MousePress(mousPos, cPGraphics);
				cPRaioExtTubo->MousePress(mousPos, cPGraphics);
				cPAlturaTubo->MousePress(mousPos, cPGraphics);
				cPDensidadeTubo->MousePress(mousPos, cPGraphics);
				cPRugosMediaTubo->MousePress(mousPos, cPGraphics);
			}
			else if (cFogueteData.cTipoPartes[cPSelectorPartesCorpo->GetSelectedOptionID()] == Foguete::TRANSICAOAUMENTO
				|| cFogueteData.cTipoPartes[cPSelectorPartesCorpo->GetSelectedOptionID()] == Foguete::TRANSICAODIMINUI)
			{
				cPRaioIntTopoTrans->MousePress(mousPos, cPGraphics);
				cPRaioIntBaseTrans->MousePress(mousPos, cPGraphics);
				cPRaioExtTopoTrans->MousePress(mousPos, cPGraphics);
				cPRaioExtBaseTrans->MousePress(mousPos, cPGraphics);
				cPAlturaTrans->MousePress(mousPos, cPGraphics);
				cPDensidadeTrans->MousePress(mousPos, cPGraphics);
				cPRugosMediaTrans->MousePress(mousPos, cPGraphics);
			}
			break;
		}
	case MOUSERELEASE:
		{
			cPSelectorPartesCorpo->MouseRelease(mousPos, cPGraphics);
			cPButAddTubo->MouseRelease(mousPos, cPGraphics);
			cPButAddTran->MouseRelease(mousPos, cPGraphics);
			cPButDelPart->MouseRelease(mousPos, cPGraphics);
			break;
		}
	case CHARPRESS:
		{			
			if (cFogueteData.cTipoPartes[cPSelectorPartesCorpo->GetSelectedOptionID()] == Foguete::TUBO)
			{
				cPRaioIntTubo->KeyPress(key);
				cPRaioExtTubo->KeyPress(key);
				cPAlturaTubo->KeyPress(key);
				cPDensidadeTubo->KeyPress(key);
				cPRugosMediaTubo->KeyPress(key);
			}
			else if (cFogueteData.cTipoPartes[cPSelectorPartesCorpo->GetSelectedOptionID()] == Foguete::TRANSICAOAUMENTO ||
				cFogueteData.cTipoPartes[cPSelectorPartesCorpo->GetSelectedOptionID()] == Foguete::TRANSICAODIMINUI)
			{
				cPRaioIntTopoTrans->KeyPress(key);
				cPRaioIntBaseTrans->KeyPress(key);
				cPRaioExtTopoTrans->KeyPress(key);
				cPRaioExtBaseTrans->KeyPress(key);
				cPAlturaTrans->KeyPress(key);
				cPDensidadeTrans->KeyPress(key);
				cPRugosMediaTrans->KeyPress(key);
			}
			break;
		}
	}
}
void RocketBuilder::HandleSubAleta(TIPO t, const DirectX::XMFLOAT2& mousPos, char key)
{
	switch (t)
	{
	case MOUSEPRESS:
		{
			cPNumAletas->MousePress(mousPos, cPGraphics);
			cPCordaRaizAle->MousePress(mousPos, cPGraphics);
			cPCordaPontaAle->MousePress(mousPos, cPGraphics);
			cPDeslocCordaPontaAle->MousePress(mousPos, cPGraphics);
			cPEnvergaduraAle->MousePress(mousPos, cPGraphics);
			cPPosVerticalAle->MousePress(mousPos, cPGraphics);
			cPEspessuraAle->MousePress(mousPos, cPGraphics);
			cPRugosidadeAle->MousePress(mousPos, cPGraphics);
			cPDensidadeAle->MousePress(mousPos, cPGraphics);
			break;
		}
	case MOUSERELEASE:
		{

			break;
		}
	case CHARPRESS:
		{
			cPNumAletas->KeyPress(key);
			cPCordaRaizAle->KeyPress(key);
			cPCordaPontaAle->KeyPress(key);
			cPDeslocCordaPontaAle->KeyPress(key);
			cPEnvergaduraAle->KeyPress(key);
			cPPosVerticalAle->KeyPress(key);
			cPEspessuraAle->KeyPress(key);
			cPRugosidadeAle->KeyPress(key);
			cPDensidadeAle->KeyPress(key);
			break;
		}
	}
}
void RocketBuilder::HandleSubMotor(TIPO t, const DirectX::XMFLOAT2& mousPos, char key)
{
	switch (t)
	{
	case MOUSEPRESS:
		{
			cPNomeArqMotor->MousePress(mousPos, cPGraphics);
			cPSearchMotorFileBut->MousePress(mousPos, cPGraphics);
			break;
		}
	case MOUSERELEASE:
		{
			cPSearchMotorFileBut->MouseRelease(mousPos, cPGraphics);
			break;
		}
	case CHARPRESS:
		{
			cPNomeArqMotor->KeyPress(key);
			break;
		}
	}

	cPSearchMotorFileBut->Pressionado();
}
void RocketBuilder::HandleSubParaq(TIPO t, const DirectX::XMFLOAT2& mousPos, char key)
{
	switch (t)
	{
	case MOUSEPRESS:
		{
			cPBoxDiamParaq->MousePress(mousPos, cPGraphics);
			cPBoxCDParaq->MousePress(mousPos, cPGraphics);
			cPBoxPosVertParaq->MousePress(mousPos, cPGraphics);
			cPBoxMassaParaq->MousePress(mousPos, cPGraphics);
			cPBoxTempoEjecaoParaq->MousePress(mousPos, cPGraphics);
			break;
		}
	case MOUSERELEASE:
		{
			
			break;
		}
	case CHARPRESS:
		{
			cPBoxDiamParaq->KeyPress(key);
			cPBoxCDParaq->KeyPress(key);
			cPBoxPosVertParaq->KeyPress(key);
			cPBoxMassaParaq->KeyPress(key);
			cPBoxTempoEjecaoParaq->KeyPress(key);
			break;
		}
	}
}
void RocketBuilder::HandleSubOptio(TIPO t, const DirectX::XMFLOAT2& mousPos, char key)
{
	switch (t)
	{
	case MOUSEPRESS:
		{
			cPButAbrirFoguete->MousePress(mousPos, cPGraphics);
			cPButSalvarFoguete->MousePress(mousPos, cPGraphics);
			cPButReAbrirFoguete->MousePress(mousPos, cPGraphics);
			cPButSalvarComoFoguete->MousePress(mousPos, cPGraphics);
			break;
		}
	case MOUSERELEASE:
		{
			cPButAbrirFoguete->MouseRelease(mousPos, cPGraphics);
			cPButSalvarFoguete->MouseRelease(mousPos, cPGraphics);
			cPButReAbrirFoguete->MouseRelease(mousPos, cPGraphics);
			cPButSalvarComoFoguete->MouseRelease(mousPos, cPGraphics);
			break;
		}
	case CHARPRESS:
		{

			break;
		}
	}
}

void RocketBuilder::SaveFile(const std::string& filePathName)
{
	/*RocketData rd;
	rd.cNomeFoguete = cPNomeFoguete->GetText();
	rd.cData.cNomeFogueteSize = cPNomeFoguete->GetText().size();

	rd.cData.cAlturaCoifa = std::atof(cPAlturaCoif->GetText().c_str()) / 100.0;
	rd.cData.cAlturaCorpo = std::atof(cPAlturaCorpo->GetText().c_str()) / 100.0;
	rd.cData.cAlturaFoguete = rd.cData.cAlturaCoifa + rd.cData.cAlturaCorpo;
	rd.cData.cCordaPontaAleta = std::atof(cPCordaPontaAle->GetText().c_str()) / 100.0;
	rd.cData.cCordaRaizAleta = std::atof(cPCordaRaizAle->GetText().c_str()) / 100.0;
	rd.cData.cDeslocamentoCordaRaizAletas = std::atof(cPDeslocCordaPontaAle->GetText().c_str()) / 100.0;
	rd.cData.cEnvergaduraAletas = std::atof(cPEnvergaduraAle->GetText().c_str()) / 100.0;
	rd.cData.cEspessuraAletas = std::atof(cPEspessuraAle->GetText().c_str()) / 100.0;

	rd.cData.cDensidadeAleta = std::atof(cPDensidadeAle->GetText().c_str());
	rd.cData.cDensidadeCoifa = std::atof(cPDensidadeCoif->GetText().c_str());
	rd.cData.cDensidadeCorpo = std::atof(cPDensidadeCorpo->GetText().c_str());

	rd.cData.cRugosidadeAleta = std::atof(cPRugosidadeAle->GetText().c_str())/ 1000000.0;
	rd.cData.cRugosidadeCoifa = std::atof(cPRugosMediaCoif->GetText().c_str()) / 1000000.0;
	rd.cData.cRugosidadeCorpo = std::atof(cPRugosMediaCorpo->GetText().c_str()) / 1000000.0;

	rd.cData.cCMXCorpo = (rd.cData.cAlturaCorpo / 2.0f) + rd.cData.cAlturaCoifa;
	rd.cData.cCMXCoifa = rd.cData.cAlturaCoifa / 2.0f;
	rd.cData.cCMXAleta = 0.0f;

	rd.cData.cNumeroAletas = std::atoi(cPNumAletas->GetText().c_str());

	rd.cPathFileMotor = cPNomeArqMotor->GetText();
	rd.cData.cPathFileMotorSize = cPNomeArqMotor->GetText().size();
	rd.cData.cPosicaoYAletas = rd.cData.cAlturaFoguete - std::atof(cPDistBaseAle->GetText().c_str()) / 100.0;
	rd.cData.cRaioBaseCoifa = std::atof(cPRaioExterCoif->GetText().c_str()) / 100.0;
	rd.cData.cRaioBaseCoifaInt = std::atof(cPRaioInterCoif->GetText().c_str()) / 100.0;
	rd.cData.cRaioCorpo = std::atof(cPRaioExtCorpo->GetText().c_str()) / 100.0;
	rd.cData.cRaioCorpoIn = std::atof(cPRaioIntCorpo->GetText().c_str()) / 100.0;
	rd.cData.cTipoCoifa = CONICA;

	rd.cData.cAreaBaseCoifa = rd.cData.cRaioBaseCoifa*rd.cData.cRaioBaseCoifa * M_PI;

	rd.cData.cAreaBaseCorpo = rd.cData.cRaioCorpo * rd.cData.cRaioCorpo * M_PI;

	if (rd.cData.cTipoCoifa == CONICA)
	{
		rd.cData.cVolumeCoifaAparente = M_PI * rd.cData.cRaioBaseCoifa * rd.cData.cRaioBaseCoifa *
			rd.cData.cAlturaCoifa / 3.0;
		rd.cData.cAreaLateralCoifa = M_PI * rd.cData.cRaioBaseCoifa * (rd.cData.cRaioBaseCoifa + sqrt((rd.cData.cRaioBaseCoifa * rd.cData.cRaioBaseCoifa) + (rd.cData.cAlturaCoifa * rd.cData.cAlturaCoifa)));

		rd.cData.cVolumeCoifaReal = rd.cData.cVolumeCoifaAparente - (M_PI * rd.cData.cRaioBaseCoifaInt * rd.cData.cRaioBaseCoifaInt *
			rd.cData.cAlturaCoifa / 3.0);
	}		
	else
		assert(0);

	rd.cData.cAreaLateralCorpo = rd.cData.cAlturaCorpo * M_PI * rd.cData.cRaioCorpo * 2.0;

	rd.cData.cComprimentoEnvergaduraAleta = sqrt((rd.cData.cEnvergaduraAletas*rd.cData.cEnvergaduraAletas) +
		pow((0.5*(rd.cData.cCordaPontaAleta - rd.cData.cCordaRaizAleta) + rd.cData.cDeslocamentoCordaRaizAletas), 2));

	rd.cData.cAreaLateralAleta = (rd.cData.cCordaPontaAleta + rd.cData.cCordaRaizAleta) * rd.cData.cEnvergaduraAletas * 0.5;
	rd.cData.cTauLAletas = atan(rd.cData.cDeslocamentoCordaRaizAletas / rd.cData.cEnvergaduraAletas);
	rd.cData.cTauCAletas = acos(rd.cData.cEnvergaduraAletas / rd.cData.cComprimentoEnvergaduraAleta);
	rd.cData.cARAletas = rd.cData.cAreaLateralAleta / rd.cData.cCordaRaizAleta;
	
	rd.cData.cYCPAletas = rd.cData.cRaioCorpo + ((rd.cData.cEnvergaduraAletas / 3.0)*
		(rd.cData.cCordaRaizAleta + (2.0*rd.cData.cCordaPontaAleta) / (rd.cData.cCordaRaizAleta + rd.cData.cCordaRaizAleta)));

	rd.cData.cVolumeCorpoAparente = rd.cData.cAreaBaseCorpo * rd.cData.cAlturaCorpo;
	rd.cData.cVolumeCorpoReal = rd.cData.cVolumeCorpoAparente - ((rd.cData.cRaioCorpoIn * rd.cData.cRaioCorpoIn * M_PI) * rd.cData.cAlturaCorpo);
	rd.cData.cAlturaFoguete = rd.cData.cAlturaCoifa + rd.cData.cAlturaCorpo;
	rd.cData.cVolumeFogueteAparente = rd.cData.cVolumeCoifaAparente + rd.cData.cVolumeCorpoAparente;
	rd.cData.cAreaLateralFoguete = rd.cData.cAreaLateralCorpo + rd.cData.cAreaLateralCoifa;
	
	rd.cData.cFinenessRatio = rd.cData.cAlturaFoguete / (2.0*rd.cData.cRaioCorpo);
	
	rd.cData.cXCPAletas = ((rd.cData.cDeslocamentoCordaRaizAletas / 3.0) *
		((rd.cData.cCordaRaizAleta + (2.0 * rd.cData.cCordaPontaAleta)) / (rd.cData.cCordaPontaAleta + rd.cData.cCordaRaizAleta)))
		+
		((rd.cData.cCordaRaizAleta + rd.cData.cCordaPontaAleta - ((rd.cData.cCordaPontaAleta*rd.cData.cCordaRaizAleta) /
		(rd.cData.cCordaRaizAleta + rd.cData.cCordaPontaAleta))) / 6.0);

	rd.cData.cXCPAletas += rd.cData.cAlturaFoguete - rd.cData.cCordaRaizAleta - (cRockData.GetData().cAlturaFoguete - cRockData.GetData().cPosicaoYAletas);

	rd.cData.cXCPCorpoECoifa = ((rd.cData.cAreaBaseCorpo * rd.cData.cAlturaFoguete) - rd.cData.cVolumeFogueteAparente) / (rd.cData.cAreaBaseCorpo - 0.0);

	rd.cData.cVolumeAleta = rd.cData.cEnvergaduraAletas * (rd.cData.cCordaPontaAleta + rd.cData.cCordaRaizAleta) / 2.0f * rd.cData.cEspessuraAletas;

	double nAlphaFins = (2.0* M_PI * ((cRockData.GetData().cEnvergaduraAletas * cRockData.GetData().cEnvergaduraAletas)))
		/ (1.0 + sqrt(1.0 + pow((cRockData.GetData().cEnvergaduraAletas * cRockData.GetData().cEnvergaduraAletas) / (cRockData.GetData().cAreaLateralAleta*cos(cRockData.GetData().cTauCAletas)), 2)));

	if (cRockData.GetData().cNumeroAletas < 3)
	{
		nAlphaFins = nAlphaFins;
	}
	else if (cRockData.GetData().cNumeroAletas == 3)
	{
		const double theta = 0.0;
		nAlphaFins = nAlphaFins * 1.5 * (1.0 - (0.15*pow(cos(3.0 / 2.0*theta), 2)));
	}
	else if (cRockData.GetData().cNumeroAletas == 4)
	{
		const double theta = 0.0;
		nAlphaFins = nAlphaFins * 2.0 * (1.0 - (0.06*pow(sin(2.0*theta), 2)));
	}
	else if (cRockData.GetData().cNumeroAletas == 5)
	{
		nAlphaFins = nAlphaFins * 2.37;
	}
	else if (cRockData.GetData().cNumeroAletas == 6)
	{
		nAlphaFins = nAlphaFins * 2.74;
	}
	else if (cRockData.GetData().cNumeroAletas == 7)
	{
		nAlphaFins = nAlphaFins * 2.99;
	}
	else
	{
		nAlphaFins = nAlphaFins * (cRockData.GetData().cNumeroAletas  * 0.427);
	}

	const double korretioneFinBodyInterference = 1.0 + (cRockData.GetData().cRaioCorpo / (cRockData.GetData().cEnvergaduraAletas + cRockData.GetData().cRaioCorpo));

	nAlphaFins *= korretioneFinBodyInterference;

	double nAlphaCorpoECoifa = 2.0 * (cRockData.GetData().cAreaBaseCorpo - 0.0);

	//cNAlphaCorpoECoifa += 1.1 * cRockData.cAreaLateralFoguete;

	double xCPFoguete = ((cRockData.GetData().cXCPAletas * nAlphaFins) + (cRockData.GetData().cXCPCorpoECoifa * nAlphaCorpoECoifa))
		/ (nAlphaFins + nAlphaCorpoECoifa);

	rd.cData.cXCPIniFoguete = xCPFoguete;

	rd.cData.cMassaSemCombustivel = (rd.cData.cDensidadeAleta * rd.cData.cVolumeAleta * rd.cData.cNumeroAletas) + (rd.cData.cDensidadeCoifa * rd.cData.cVolumeCoifaReal) + (rd.cData.cDensidadeCorpo * rd.cData.cVolumeCorpoReal);
	rd.cData.cMomentoInerciaFoguete = -1.0;*/
	
	cFogueteData.Salvar(filePathName);
}

void RocketBuilder::OpenFile(const std::string& filePathName)
{
	cPRocketModel->DeletarTodasTransicoes();
	cPSelectorPartesCorpo->DeleteAllOptions();

	cFogueteData.Abrir(filePathName);
	
	for (size_t i = 0; i < cFogueteData.cTipoPartes.size(); i++)
	{
		switch (cFogueteData.cTipoPartes[i])
		{
		case Foguete::TUBO:
		{
			cPSelectorPartesCorpo->AddOption(std::to_string(i) + ". Tubo");
		}
			break;
		case Foguete::TRANSICAOAUMENTO:
		case Foguete::TRANSICAODIMINUI:
		{
			cPSelectorPartesCorpo->AddOption(std::to_string(i) + ". Transicao");
			cPRocketModel->AdicionarTransicao((float)cFogueteData.cPartesCorpo[i]->PegarAltura(), 
				(float)cFogueteData.cPartesCorpo[i]->PegarRaioBaseExt(), (float)cFogueteData.cPartesCorpo[i]->PegarRaioTopoExt(),
				i, cPGraphics);
		}			
			break;
		case Foguete::COIFACONICA:
		{
			cPSelectorPartesCorpo->AddOption(std::to_string(i) + ". Coifa Conica");
		}
			break;
		}
	}
		
	cPCordaPontaAle->SetText(std::to_string(cFogueteData.cAletas[0]->PegarCordaPonta() * 100.0));
	cPCordaPontaAle->ClearZeros();

	cPCordaRaizAle->SetText(std::to_string(cFogueteData.cAletas[0]->PegarCordaRaiz()  * 100.0));
	cPCordaRaizAle->ClearZeros();

	cPDensidadeAle->SetText(std::to_string(cFogueteData.cAletas[0]->PegarDensidade()));
	cPDensidadeAle->ClearZeros();

	cPDeslocCordaPontaAle->SetText(std::to_string(cFogueteData.cAletas[0]->PegarDeslCordaPonta() * 100.0));
	cPDeslocCordaPontaAle->ClearZeros();

	cPEnvergaduraAle->SetText(std::to_string(cFogueteData.cAletas[0]->PegarEnvergadura() * 100.0));
	cPEnvergaduraAle->ClearZeros();	
		
	cPEspessuraAle->SetText(std::to_string(cFogueteData.cAletas[0]->PegarEspessura() * 100.0));
	cPEspessuraAle->ClearZeros();

	cPNumAletas->SetText(std::to_string(cFogueteData.cAletas[0]->PegarNumAletas()));

	cPRugosidadeAle->SetText(std::to_string(cFogueteData.cAletas[0]->PegarRugosidade() * 1000000.0));
	cPRugosidadeAle->ClearZeros();

	cPNomeFoguete->SetText(cFogueteData.cNomeFoguete);

	cPNomeArqMotor->SetText(cFogueteData.cPMotor->PegarCaminhoMotor());

	cPPosVerticalAle->SetText(std::to_string((cFogueteData.cAletas[0]->PegarPosV() * 100.0)));
	cPPosVerticalAle->ClearZeros();

	cPAlturaCoif->SetText(std::to_string(cFogueteData.cPartesCorpo[0]->PegarAltura() * 100.0));
	cPAlturaCoif->ClearZeros();

	cPDensidadeCoif->SetText(std::to_string(cFogueteData.cPartesCorpo[0]->PegarDensidade()));
	cPDensidadeCoif->ClearZeros();

	cPRaioExterCoif->SetText(std::to_string(cFogueteData.cPartesCorpo[0]->PegarRaioBaseExt() * 100.0));
	cPRaioExterCoif->ClearZeros();

	cPRaioInterCoif->SetText(std::to_string(cFogueteData.cPartesCorpo[0]->PegarRaioBaseInt() * 100.0));
	cPRaioInterCoif->ClearZeros();
	
	cPRugosMediaCoif->SetText(std::to_string(cFogueteData.cPartesCorpo[0]->PegarRugosidade() * 1000000.0));
	cPRugosMediaCoif->ClearZeros();
	
	cPBoxDiamParaq->SetText(std::to_string(cFogueteData.cPParaquedas->PegarRaio() * 2.0 * 100.0));
	cPBoxDiamParaq->ClearZeros();

	cPBoxCDParaq->SetText(std::to_string(cFogueteData.cPParaquedas->PegarCd(cFogueteData.cPParaquedas->PegarArea())));
	cPBoxCDParaq->ClearZeros();

	cPBoxPosVertParaq->SetText(std::to_string(cFogueteData.cPParaquedas->PegarPosVert() * 100.0));
	cPBoxPosVertParaq->ClearZeros();

	cPBoxMassaParaq->SetText(std::to_string(cFogueteData.cPParaquedas->PegarMassa() * 1000.0));
	cPBoxMassaParaq->ClearZeros();

	cPBoxTempoEjecaoParaq->SetText(std::to_string(cFogueteData.cPParaquedas->PegarDelay()));
	cPBoxTempoEjecaoParaq->ClearZeros();
}

void RocketBuilder::UpdateRocketData()
{
	cFogueteData.cNomeFoguete = cPNomeFoguete->GetText();

	switch (cPMenuEsq->GetActiveButton())
	{
	case 0:
		SubMenuCoifaUpdateRocket();
		break;
	case 1:
		SubMenuCorpoUpdateRocket();
		break;
	case 2:
		SubMenuAletaUpdateRocket();
		break;
	}

	switch (cPMenuDir->GetActiveButton())
	{
	case 0:
		SubMenuMotorUpdateRocket();
		break;
	case 1:
		SubMenuParaqueUpdateRocket();
		break;
	case 2:
		SubMenuOpcoesUpdateRocket();
		break;
	}			
}

void RocketBuilder::SubMenuCoifaUpdateRocket()
{
	double AlturaCoifa = std::atof(cPAlturaCoif->GetText().c_str()) / 100.0;
	double DensidadeCoifa = std::atof(cPDensidadeCoif->GetText().c_str());
	double RugosidadeCoifa = std::atof(cPRugosMediaCoif->GetText().c_str()) / 1000000.0;
	double RaioBaseCoifa = std::atof(cPRaioExterCoif->GetText().c_str()) / 100.0;
	double RaioBaseCoifaInt = std::atof(cPRaioInterCoif->GetText().c_str()) / 100.0;

	cFogueteData.MudarCoifaConica(RugosidadeCoifa, DensidadeCoifa, RaioBaseCoifa, RaioBaseCoifaInt, AlturaCoifa, 0);
}

void RocketBuilder::SubMenuCorpoUpdateRocket()
{
	const uint32_t selectedID = cPSelectorPartesCorpo->GetSelectedOptionID();

	static uint32_t lastSelectedID = 0;

	if (selectedID != lastSelectedID)
	{
		lastSelectedID = selectedID;

		switch (cFogueteData.cTipoPartes[selectedID])
		{
		case Foguete::TUBO:
		{
			cPAlturaTubo->SetText(std::to_string(cFogueteData.cPartesCorpo[selectedID]->PegarAltura() * 100.0));
			cPAlturaTubo->ClearZeros();

			cPRaioExtTubo->SetText(std::to_string(cFogueteData.cPartesCorpo[selectedID]->PegarRaioBaseExt() * 100.0));
			cPRaioExtTubo->ClearZeros();

			cPRaioIntTubo->SetText(std::to_string(cFogueteData.cPartesCorpo[selectedID]->PegarRaioBaseInt() * 1e2));
			cPRaioIntTubo->ClearZeros();

			cPRugosMediaTubo->SetText(std::to_string(cFogueteData.cPartesCorpo[selectedID]->PegarRugosidade() * 1e6));
			cPRugosMediaTubo->ClearZeros();

			cPDensidadeTubo->SetText(std::to_string(cFogueteData.cPartesCorpo[selectedID]->PegarDensidade()));
			cPDensidadeTubo->ClearZeros();
		}
		break;
		case Foguete::TRANSICAOAUMENTO:
		case Foguete::TRANSICAODIMINUI:
		{
			cPAlturaTrans->SetText(std::to_string(cFogueteData.cPartesCorpo[selectedID]->PegarAltura() * 100.0));
			cPAlturaTrans->ClearZeros();

			cPRaioExtTopoTrans->SetText(std::to_string(cFogueteData.cPartesCorpo[selectedID]->PegarRaioTopoExt() * 100.0));
			cPRaioExtTopoTrans->ClearZeros();

			cPRaioIntTopoTrans->SetText(std::to_string(cFogueteData.cPartesCorpo[selectedID]->PegarRaioTopoInt() * 100.0));
			cPRaioIntTopoTrans->ClearZeros();

			cPRaioExtBaseTrans->SetText(std::to_string(cFogueteData.cPartesCorpo[selectedID]->PegarRaioBaseExt() * 100.0));
			cPRaioExtBaseTrans->ClearZeros();

			cPRaioIntBaseTrans->SetText(std::to_string(cFogueteData.cPartesCorpo[selectedID]->PegarRaioBaseInt() * 100.0));
			cPRaioIntBaseTrans->ClearZeros();
			
			cPRugosMediaTrans->SetText(std::to_string(cFogueteData.cPartesCorpo[selectedID]->PegarRugosidade() * 1e6));
			cPRugosMediaTrans->ClearZeros();

			cPDensidadeTrans->SetText(std::to_string(cFogueteData.cPartesCorpo[selectedID]->PegarDensidade()));
			cPDensidadeTrans->ClearZeros();

		}
		break;
		}
	}

	switch (cFogueteData.cTipoPartes[selectedID])
	{
	case Foguete::TUBO:
	{
		double AlturaCorpo = std::atof(cPAlturaTubo->GetText().c_str()) / 100.0;
		double RaioCorpo =  std::atof(cPRaioExtTubo->GetText().c_str()) / 100.0;
		double RaioCorpoIn = std::atof(cPRaioIntTubo->GetText().c_str()) / 100.0;
		double RugosidadeCorpo = std::atof(cPRugosMediaTubo->GetText().c_str()) / 1000000.0;
		double DensidadeCorpo = std::atof(cPDensidadeTubo->GetText().c_str());

		cFogueteData.MudarTubo(RugosidadeCorpo, DensidadeCorpo, RaioCorpo, RaioCorpoIn, AlturaCorpo, selectedID);
	}
		break;
	case Foguete::TRANSICAOAUMENTO:
	case Foguete::TRANSICAODIMINUI:
	{
		static double ultAlturaCorpo = 0.0;
		static double ultRaioCorpoExTopo = 0.0;
		static double ultRaioCorpoExBase = 0.0;
		
		double alturaCorpo = std::atof(cPAlturaTrans->GetText().c_str()) / 100.0;
		double raioCorpoExTopo = std::atof(cPRaioExtTopoTrans->GetText().c_str()) / 100.0;
		double raioCorpoInTopo = std::atof(cPRaioIntTopoTrans->GetText().c_str()) / 100.0;
		double raioCorpoExBase = std::atof(cPRaioExtBaseTrans->GetText().c_str()) / 100.0;
		double raioCorpoInBase = std::atof(cPRaioIntBaseTrans->GetText().c_str()) / 100.0;
		

		if (alturaCorpo != ultAlturaCorpo || raioCorpoExTopo != ultRaioCorpoExTopo 
			|| raioCorpoExBase != ultRaioCorpoExBase)
		{
			ultAlturaCorpo = alturaCorpo;
			ultRaioCorpoExTopo = raioCorpoExTopo;
			ultRaioCorpoExBase = raioCorpoExBase;

			cPRocketModel->UpdateTransicao(selectedID, (float)alturaCorpo, (float)raioCorpoExBase, (float)raioCorpoExTopo, cPGraphics);
		}

		double rugosidadeCorpo = std::atof(cPRugosMediaTrans->GetText().c_str()) / 1000000.0;
		double densidadeCorpo = std::atof(cPDensidadeTrans->GetText().c_str());

		cFogueteData.MudarTransicao(rugosidadeCorpo, densidadeCorpo, raioCorpoExTopo, raioCorpoInTopo,
			raioCorpoExBase, raioCorpoInBase, alturaCorpo, selectedID);
	}
		break;
	}		
}

void RocketBuilder::SubMenuAletaUpdateRocket()
{
	double CordaPontaAleta = std::atof(cPCordaPontaAle->GetText().c_str()) / 100.0;
	double CordaRaizAleta = std::atof(cPCordaRaizAle->GetText().c_str()) / 100.0;
	double DeslocamentoCordaRaizAletas = std::atof(cPDeslocCordaPontaAle->GetText().c_str()) / 100.0;
	double EnvergaduraAletas = std::atof(cPEnvergaduraAle->GetText().c_str()) / 100.0;
	double EspessuraAletas = std::atof(cPEspessuraAle->GetText().c_str()) / 100.0;
	double DensidadeAleta = std::atof(cPDensidadeAle->GetText().c_str());
	double RugosidadeAleta = std::atof(cPRugosidadeAle->GetText().c_str()) / 1000000.0;
	double posVerticalAle = std::atof(cPPosVerticalAle->GetText().c_str()) / 100.0;
	int NumeroAletas = std::atoi(cPNumAletas->GetText().c_str());

	cFogueteData.MudarAletaTrapezoidal(RugosidadeAleta, EspessuraAletas, DensidadeAleta, NumeroAletas, Aleta::ARREDONDADO,
		CordaRaizAleta, CordaPontaAleta, EnvergaduraAletas, DeslocamentoCordaRaizAletas,
		posVerticalAle, 0);
}
	 
void RocketBuilder::SubMenuMotorUpdateRocket()
{
	if (!cPNomeArqMotor->GetText()._Equal(cUltNomeMotor))
	{
		cFogueteData.DefinirMotor(cPNomeArqMotor->GetText(), cFogueteData.cAlturaFoguete);
		cUltNomeMotor = cPNomeArqMotor->GetText();
	}
}

void RocketBuilder::SubMenuParaqueUpdateRocket()
{
	const double diametro = std::atof(cPBoxDiamParaq->GetText().c_str()) / 100.0;
	const double cd = std::atof(cPBoxCDParaq->GetText().c_str());
	const double posVerPara = std::atof(cPBoxPosVertParaq->GetText().c_str()) / 100.0;
	const double massa = std::atof(cPBoxMassaParaq->GetText().c_str()) / 1000.0;
	const double tempoEje = std::atof(cPBoxTempoEjecaoParaq->GetText().c_str());

	cFogueteData.DefinirParaquedas(tempoEje, cd, diametro, massa, posVerPara);
}

void RocketBuilder::SubMenuOpcoesUpdateRocket()
{

}

template<class T>
void RocketBuilder::Delete(T** p)
{
	if (*p)
	{
		delete *p;
		*p = nullptr;
	}
}