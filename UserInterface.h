#pragma once
#include "Mouse.h"
#include "UIImage.h"

class UserInterface
{
public:
	UserInterface();
	~UserInterface();

	void Draw(GraphicsDxD* pGfx) const;

	void Tick();

private:

};