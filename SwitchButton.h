#pragma once
#include "Button.h"

class SwitchButton : public Button
{
public:
	SwitchButton(const ButtonDesc& butDesc, const std::wstring& filenameSwitch, GraphicsDxD* pGfx)
		:
		Button(butDesc, pGfx),
		cPImgSwitch(nullptr)
	{
		cPImgSwitch = new Image(filenameSwitch, pGfx);
	}
	~SwitchButton()
	{
		if (cPImgSwitch)
		{
			delete cPImgSwitch;
			cPImgSwitch = nullptr;
		}
	}
	
	void MousePress(const DirectX::XMFLOAT2& posMouse, const GraphicsDxD* pGfx) override
	{
		const float wndWidth = (float)pGfx->GetWndWidth();
		const float wndHeight = (float)pGfx->GetWndHeight();

		const RectBut ret = { cRetCol.x0 * wndWidth, cRetCol.y0 * wndHeight, cRetCol.x1 * wndWidth, cRetCol.y1 * wndHeight };

		if (ret.Contem(posMouse))
		{
			cPressionado = true;
			if (cSwitchState == false)
				cSwitchState = true;
			else
				cSwitchState = false;
		}
	}

	void Draw(const DirectX::XMFLOAT2& posMouse, GraphicsDxD* pGfx) const override
	{
		const float wndWidth = (float)pGfx->GetWndWidth();
		const float wndHeight = (float)pGfx->GetWndHeight();

		const RectBut ret = { cRetCol.x0 * wndWidth, cRetCol.y0 * wndHeight, cRetCol.x1 * wndWidth, cRetCol.y1 * wndHeight };
		const DirectX::XMFLOAT2 pos = { (cPos.x * wndWidth) - (wndWidth / 2.0f) + (cSize.x*wndWidth / 2.0f),
			(cPos.y * wndHeight) - (wndHeight / 2.0f) + (cSize.y * wndHeight / 2.0f) };
		if (cSwitchState)
		{
			cPImgSwitch->Draw(pos, 0.0f, { cSize.x * wndWidth, cSize.y * wndHeight }, DirectX::XMMatrixIdentity(), pGfx);
		}		
		else if (cPressionado)
		{
			cPImgPressed->Draw(pos, 0.0f, { cSize.x * wndWidth, cSize.y * wndHeight }, DirectX::XMMatrixIdentity(), pGfx);
		}
		else if (ret.Contem(posMouse))
		{
			cPImgRoll->Draw(pos, 0.0f, { cSize.x * wndWidth, cSize.y * wndHeight }, DirectX::XMMatrixIdentity(), pGfx);
		}
		else
		{
			cPImgDefault->Draw(pos, 0.0f, { cSize.x * wndWidth, cSize.y * wndHeight }, DirectX::XMMatrixIdentity(), pGfx);
		}
	}

	void SetState(bool state)
	{
		cSwitchState = state;
	}

	bool GetState() const
	{
		return cSwitchState;
	}

private:
	bool cSwitchState;
	Image* cPImgSwitch;
};