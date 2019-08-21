#pragma once
#include "Button.h"

class PushButton : public Button
{
public:
	PushButton(const ButtonDesc& butDesc, GraphicsDxD* pGfx)
		:
		Button(butDesc, pGfx),
		cAlreadyPushed(false)
	{}
	~PushButton()
	{}

	bool AlreadyPushed() const
	{
		return cAlreadyPushed;
	}

	void MousePress(const DirectX::XMFLOAT2& posMouse, const GraphicsDxD* pGfx) override
	{
		const float wndWidth = (float)pGfx->GetWndWidth();
		const float wndHeight =(float) pGfx->GetWndHeight();

		const RectBut ret = { cRetCol.x0 * wndWidth, cRetCol.y0 * wndHeight, cRetCol.x1 * wndWidth, cRetCol.y1 * wndHeight };

		if (ret.Contem(posMouse))
		{
			cPressionado = true;
			cAlreadyPushed = true;
		}
	}
	
	void ReSet()
	{
		cAlreadyPushed = false;
	}

private:
	bool cAlreadyPushed;
};