#pragma once
#include "Keyboard.h"
#include "Mouse.h"
#include "GraphicsDxd.h"

#include "RocketBuilder.h"
#include "RocketSimulator.h"

enum MEN_ID
{
	MEN_MAN,
	MEN_BUI,
	MEN_SIM,
	MEN_STU
};

class Program
{
public:
	Program(KeyboardClient& kbd, MouseClient& mouse, GraphicsDxD* pGfx);
	~Program();

	void Tick();	

private:
	RocketBuilder cRocketBuilder;
	RocketSimulator cRocketSim;
	
	MEN_ID cMenuAtual;
};