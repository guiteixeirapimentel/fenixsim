#include "Menu.h"

Menu::Menu()
	:
	cActiveButtonIndex(-1)
{}

Menu::~Menu()
{
	for (size_t i = 0; i < cButtonsPointers.size(); i++)
	{
		delete cButtonsPointers[i];
		cButtonsPointers[i] = nullptr;
	}
}

void Menu::AddButton(const ButtonDesc& butDesc, const std::wstring& filenameSwitch, GraphicsDxD* pGfx)
{
	SwitchButton* pSwiBtn = new SwitchButton(butDesc, filenameSwitch, pGfx);
	cButtonsPointers.push_back(pSwiBtn);

	if (cActiveButtonIndex == -1)
	{
		cActiveButtonIndex = 0;
		cButtonsPointers.back()->SetState(true);
	}
}

void Menu::MousePress(const DirectX::XMFLOAT2 posMouse, GraphicsDxD* pGfx)
{
	int ult = cActiveButtonIndex;
	for (size_t i = 0; i < cButtonsPointers.size(); i++)
	{
		cButtonsPointers[i]->MousePress(posMouse, pGfx);
		cButtonsPointers[i]->SetState(false);

		if (cButtonsPointers[i]->Pressionado())
		{
			cActiveButtonIndex = i;
			cButtonsPointers[i]->SetState(true);
		}
	}

	if (ult == cActiveButtonIndex)
	{
		cButtonsPointers[ult]->SetState(true);
	}
}

void Menu::Draw(const DirectX::XMFLOAT2 posMouse, GraphicsDxD* pGfx)
{
	for (size_t i = 0; i < cButtonsPointers.size(); i++)
	{
		cButtonsPointers[i]->Draw(posMouse, pGfx);
	}
}

void Menu::MouseRelease(const DirectX::XMFLOAT2 posMouse, GraphicsDxD* pGfx)
{
	for (size_t i = 0; i < cButtonsPointers.size(); i++)
	{
		cButtonsPointers[i]->MouseRelease(posMouse, pGfx);
	}
}