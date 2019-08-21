#include "RadioTuple.h"

RadioTuple::RadioTuple()
	:
	cMarkedButIndex(-1)
{}

RadioTuple::~RadioTuple()
{
	for (size_t i = 0; i < cButtonsPointers.size(); i++)
	{
		delete cButtonsPointers[i];
		cButtonsPointers[i] = nullptr;
	}
}

void RadioTuple::AddButton(const ButtonDesc& butDesc, const std::wstring& filenameSwitch, GraphicsDxD* pGfx)
{
	SwitchButton* pSwiBtn = new SwitchButton(butDesc, filenameSwitch, pGfx);
	cButtonsPointers.push_back(pSwiBtn);

	if (cMarkedButIndex == -1)
	{
		cMarkedButIndex = 0;
		cButtonsPointers.back()->SetState(true);
	}
}

void RadioTuple::MousePress(const DirectX::XMFLOAT2 posMouse, GraphicsDxD* pGfx)
{
	int ult = cMarkedButIndex;
	for (size_t i = 0; i < cButtonsPointers.size(); i++)
	{
		cButtonsPointers[i]->MousePress(posMouse, pGfx);
		cButtonsPointers[i]->SetState(false);

		if (cButtonsPointers[i]->Pressionado())
		{
			cMarkedButIndex = i;
			cButtonsPointers[i]->SetState(true);
		}
	}

	if (ult == cMarkedButIndex)
	{
		cButtonsPointers[ult]->SetState(true);
	}
}

void RadioTuple::Draw(const DirectX::XMFLOAT2 posMouse, GraphicsDxD* pGfx)
{
	for (size_t i = 0; i < cButtonsPointers.size(); i++)
	{
		cButtonsPointers[i]->Draw(posMouse, pGfx);
	}
}

void RadioTuple::MouseRelease(const DirectX::XMFLOAT2 posMouse, GraphicsDxD* pGfx)
{
	for (size_t i = 0; i < cButtonsPointers.size(); i++)
	{
		cButtonsPointers[i]->MouseRelease(posMouse, pGfx);
	}
}