#pragma once
#include "Button.h"
#include <queue>

class EventButton : public Button
{
public:
	EventButton(const ButtonDesc& butDesc, GraphicsDxD* pGfx)
		:
		Button(butDesc, pGfx)
	{}

	void MousePress(const DirectX::XMFLOAT2& posMouse, const GraphicsDxD* pGfx) override
	{
		const float wndWidth = (float)pGfx->GetWndWidth();
		const float wndHeight = (float)pGfx->GetWndHeight();

		const RectBut ret = { cRetCol.x0 * wndWidth, cRetCol.y0 * wndHeight, cRetCol.x1 * wndWidth, cRetCol.y1 * wndHeight };

		if (ret.Contem(posMouse))
		{
			cPressionado = true;

			if (cEvents.size() > 5)
				cEvents.pop();

			cEvents.push(true);
		}
	}

	void MouseRelease(const DirectX::XMFLOAT2& posMouse, const GraphicsDxD* pGfx) override
	{
		if (cPressionado)
		{
			cPressionado = false;
			if (cEvents.size() > 5)
				cEvents.pop();
			cEvents.push(false);
		}
	}

	bool ReturnLastState()
	{
		bool res = false;
		if (cEvents.size() > 0)
		{
			res = cEvents.back();
			cEvents.pop();
		}			
		return res;
	}

private:
	std::queue<bool> cEvents;
};