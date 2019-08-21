#pragma once
#include "Image.h"

struct RectBut
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


struct ButtonDesc
{
	std::wstring fileNameDefault;
	std::wstring fileNamePressed;
	std::wstring filenameRollover;

	DirectX::XMFLOAT2 pos;
	DirectX::XMFLOAT2 size;
	DirectX::XMFLOAT2 defWndSize;
};

class Button
{
public:
	Button(const ButtonDesc& butDesc, GraphicsDxD* pGfx);
	~Button();

	virtual void MousePress(const DirectX::XMFLOAT2& posMouse, const GraphicsDxD* pGfx) = 0;
	virtual void MouseRelease(const DirectX::XMFLOAT2& posMouse, const GraphicsDxD* pGfx)
	{
		if (cPressionado)
		{
			cPressionado = false;
		}
	}

	virtual void Draw(const DirectX::XMFLOAT2& posMouse, GraphicsDxD* pGfx) const;

	bool Pressionado() const
	{
		return cPressionado;
	}

protected:
	Image* cPImgDefault;
	Image* cPImgPressed;
	Image* cPImgRoll;

	const DirectX::XMFLOAT2 cPos;
	const DirectX::XMFLOAT2 cSize;

	RectBut cRetCol;

	bool cPressionado;
};