#pragma once
#include "Button.h"

class AdderButton : public Button
{
public:
	AdderButton(const ButtonDesc& butDesc, GraphicsDxD* pGfx)
		:
		Button(butDesc, pGfx),
		cValue(0)
	{}
	~AdderButton()
	{}
	
	void MousePress(const DirectX::XMFLOAT2& posMouse, const GraphicsDxD* pGfx) override
	{
		const float wndWidth = (float)pGfx->GetWndWidth();
		const float wndHeight = (float)pGfx->GetWndHeight();

		const RectBut ret = { cRetCol.x0 * wndWidth, cRetCol.y0 * wndHeight, cRetCol.x1 * wndWidth, cRetCol.y1 * wndHeight };

		if (ret.Contem(posMouse) && !cPressionado)
		{
			cPressionado = true;
			cValue++;
		}
	}

	int GetValue() const
	{
		return cValue;
	}

	void SetValue(int value)
	{
		cValue = value;
	}

private:
	int cValue;

};