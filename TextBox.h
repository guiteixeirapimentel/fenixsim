#pragma once
#include "Image.h"
#include "Font.h"

struct RectBox
{
	float x0;
	float y0;
	float x1;
	float y1;

	bool Contem(const DirectX::XMFLOAT2 point) const
	{
		if (point.x <= x1 && point.x >= x0 && point.y <= y1 && point.y >= y0)
			return true;
		else
			return false;
	}


};


struct BoxDesc
{
	std::wstring fileNameDefault;
	std::wstring fileNameSelected;

	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT2 size;

	DirectX::XMFLOAT2 fontSize;
	
	const Font* pFont;

	DirectX::XMFLOAT2 defWndSize;

	bool centralizado;
};

class TextBox
{
public:
	TextBox(const BoxDesc& boxDesc, GraphicsDxD* pGfx);
	~TextBox();

	void MousePress(const DirectX::XMFLOAT2& posMouse, GraphicsDxD* pGfx);

	void Draw(const DirectX::XMFLOAT2& posMouse, GraphicsDxD* pGfx);

	virtual void KeyPress(unsigned char key);

	std::string GetText() const
	{
		return cText;
	}
	void SetText(const std::string& txt)
	{
		cText = txt;
	}
	
protected:
	Image* cPImgBGDefault;
	Image* cPImgBGSelected;

	const DirectX::XMFLOAT2 cPos;
	const DirectX::XMFLOAT2 cSize;
	const DirectX::XMFLOAT2 cFontSize;

	const Font* cPFont;

	RectBox cRetCol;

	bool cSelected;
	bool cCentralized;
	
	std::string cText;
	const int cMaxLenShown;

	bool cNewText;
};