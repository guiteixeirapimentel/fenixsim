#include "Program.h"

Program::Program(KeyboardClient& kbd, MouseClient& mouse, GraphicsDxD* pGfx)
	:
	cRocketBuilder(kbd, mouse, pGfx, &cMenuAtual),
	cRocketSim(kbd, mouse, pGfx, &cMenuAtual),
	cMenuAtual(MEN_BUI)
{}

Program:: ~Program()
{}

void Program::Tick()
{
	switch (cMenuAtual)
	{
	case MEN_MAN:
		cRocketSim.Tick();
		break;
	case MEN_BUI:
		cRocketBuilder.Tick();
		break;
	case MEN_SIM:
		cRocketSim.Tick();
		break;
	case MEN_STU:
		cRocketSim.Tick();
		break;
	}	
}