#include "Button.h"

Button::Button(const ButtonDesc& butDesc, GraphicsDxD* pGfx)
	:
	cPos({ butDesc.pos.x / butDesc.defWndSize.x, butDesc.pos.y / butDesc.defWndSize.y }),
	cSize({ butDesc.size.x / butDesc.defWndSize.x, butDesc.size.y / butDesc.defWndSize.y }),
	cPImgDefault(nullptr),
	cPImgPressed(nullptr),
	cPImgRoll(nullptr)
{
	cPImgDefault = new Image(butDesc.fileNameDefault, pGfx);
	cPImgPressed = new Image(butDesc.fileNamePressed, pGfx);
	cPImgRoll = new Image(butDesc.filenameRollover, pGfx);

	cRetCol.x0 = cPos.x;
	cRetCol.y0 = cPos.y;
	cRetCol.x1 = cPos.x + cSize.x;
	cRetCol.y1 = cPos.y + cSize.y;
}

Button::~Button()
{
	if (cPImgDefault)
	{
		delete cPImgDefault;
		cPImgDefault = nullptr;
	}

	if (cPImgPressed)
	{
		delete cPImgPressed;
		cPImgPressed = nullptr;
	}

	if (cPImgRoll)
	{
		delete cPImgRoll;
		cPImgRoll = nullptr;
	}
}

void Button::Draw(const DirectX::XMFLOAT2& posMouse, GraphicsDxD* pGfx) const
{
	const float wndWidth = (float)pGfx->GetWndWidth();
	const float wndHeight =(float)pGfx->GetWndHeight();
	
	const RectBut ret = { cRetCol.x0 * wndWidth, cRetCol.y0 * wndHeight, cRetCol.x1 * wndWidth, cRetCol.y1 * wndHeight };
	const DirectX::XMFLOAT2 pos = { (cPos.x * wndWidth) - (wndWidth/2.0f) + (cSize.x*wndWidth/2.0f), 
		(cPos.y * wndHeight) - (wndHeight / 2.0f) + (cSize.y * wndHeight/2.0f) };

	if (cPressionado)
	{
		cPImgPressed->Draw(pos, 0.0f, { cSize.x * wndWidth, cSize.y * wndHeight }, DirectX::XMMatrixIdentity(), pGfx);
	}
	else if(ret.Contem(posMouse))
	{
		cPImgRoll->Draw(pos, 0.0f, { cSize.x * wndWidth, cSize.y * wndHeight }, DirectX::XMMatrixIdentity(), pGfx);
	}
	else
	{
		cPImgDefault->Draw(pos, 0.0f, { cSize.x * wndWidth, cSize.y * wndHeight }, DirectX::XMMatrixIdentity(), pGfx);
	}
}