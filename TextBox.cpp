#include "TextBox.h"

TextBox::TextBox(const BoxDesc& boxDesc, GraphicsDxD* pGfx)
	:
	cPos({ boxDesc.pos.x / boxDesc.defWndSize.x, boxDesc.pos.y / boxDesc.defWndSize.y }),
	cSize({ boxDesc.size.x / boxDesc.defWndSize.x, boxDesc.size.y / boxDesc.defWndSize.y }),
	cFontSize({ boxDesc.fontSize.x / boxDesc.defWndSize.x, boxDesc.fontSize.y / boxDesc.defWndSize.y }),
	cSelected(false),
	cMaxLenShown(static_cast<int>((cSize.x / cFontSize.x) - 0.5f)),
	cPFont(boxDesc.pFont),
	cCentralized(boxDesc.centralizado),
	cNewText(true)
{
	cPImgBGDefault = new Image(boxDesc.fileNameDefault, pGfx);
	cPImgBGSelected = new Image(boxDesc.fileNameSelected, pGfx);

	cRetCol.x0 = cPos.x;
	cRetCol.y0 = cPos.y;
	cRetCol.x1 = cPos.x + cSize.x;
	cRetCol.y1 = cPos.y + cSize.y;

	cText = "";
}

TextBox::~TextBox()
{
	if (cPImgBGDefault)
	{
		delete cPImgBGDefault;
		cPImgBGDefault = nullptr;
	}
	if (cPImgBGSelected)
	{
		delete cPImgBGSelected;
		cPImgBGSelected = nullptr;
	}
}

void TextBox::MousePress(const DirectX::XMFLOAT2& posMouse, GraphicsDxD* pGfx)
{
	const float wndWidth = (float)pGfx->GetWndWidth();
	const float wndHeight = (float)pGfx->GetWndHeight();

	const RectBox ret = { cRetCol.x0 * wndWidth, cRetCol.y0 * wndHeight, cRetCol.x1 * wndWidth, cRetCol.y1 * wndHeight };

	if (ret.Contem(posMouse))
	{
		cSelected = true;
		cNewText = true;
	}
	else
	{
		cSelected = false;
	}
}

void TextBox::Draw(const DirectX::XMFLOAT2& posMouse, GraphicsDxD* pGfx)
{
	const float wndWidth = (float)pGfx->GetWndWidth();
	const float wndHeight = (float)pGfx->GetWndHeight();

	const DirectX::XMFLOAT2 pos = { (cPos.x * wndWidth) - (wndWidth / 2.0f) + (cSize.x * wndWidth / 2.0f) , 
		(cPos.y * wndHeight) - (wndHeight / 2.0f) + (cSize.y * wndHeight / 2.0f) };

	DirectX::XMFLOAT2 posFont = { (cPos.x * wndWidth) - (wndWidth / 2.0f) + (cFontSize.x * wndWidth / 2.0f),
		(cPos.y * wndHeight) - (wndHeight / 2.0f) + (cFontSize.y * wndHeight / 2.0f) };

	Font::ALIGN al = Font::LEFT;

	if (cCentralized)
	{
		posFont.x += (cSize.x / 2.0f) * wndWidth;
		posFont.y -= (cSize.y / 4.0f) * wndHeight;
		al = Font::CENTER;
	}

	if (!cSelected)
	{
		cPImgBGDefault->Draw(pos, 0.0f,
		{ cSize.x * wndWidth, cSize.y * wndHeight }, DirectX::XMMatrixIdentity(), pGfx);
	}
	else
	{
		cPImgBGSelected->Draw(pos, 0.0f,
		{ cSize.x * wndWidth, cSize.y * wndHeight }, DirectX::XMMatrixIdentity(), pGfx);
	}
	if (static_cast<int>(cText.length()) < cMaxLenShown)
	{
		cPFont->Write(cText, posFont, { cFontSize.x * wndWidth, cFontSize.y * wndHeight },
			al, pGfx);
	}
	else
	{
		cPFont->Write(&(cText[cText.length() - cMaxLenShown]), posFont, { cFontSize.x * wndWidth, cFontSize.y * wndHeight },
			al, pGfx);
	}
}

void TextBox::KeyPress(unsigned char c)
{
	if (cSelected)
	{
		if (c >= 32 && c <= 126)
		{
			if (cNewText)
			{
				cText.clear();
				cNewText = false;
			}
			cText += c;
		}
		else if (c == VK_BACK && cText.length() > 0)
		{
			cText.pop_back();
			cNewText = false;
		}
		else if (c == VK_RETURN)
		{
			cSelected = false;
		}
	}
}