#pragma once
#include "SwitchButton.h"
#include <vector>

class RadioTuple
{
public:
	RadioTuple();
	~RadioTuple();

	void AddButton(const ButtonDesc& butDesc, const std::wstring& filenameSwitch, GraphicsDxD* pGfx);

	void MousePress(const DirectX::XMFLOAT2 posMouse, GraphicsDxD* pGfx);

	void Draw(const DirectX::XMFLOAT2 posMouse, GraphicsDxD* pGfx);

	void MouseRelease(const DirectX::XMFLOAT2 posMouse, GraphicsDxD* pGfx);

	int GetMarkedButton() const
	{
		return cMarkedButIndex;
	}

private:
	std::vector<SwitchButton*> cButtonsPointers;

	int cMarkedButIndex;
};