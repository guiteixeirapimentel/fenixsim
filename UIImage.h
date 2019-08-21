#pragma once
#include "Image.h"

class UIImage
{
public:
	UIImage(const std::wstring& fileName, const DirectX::XMFLOAT2& size, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& defWndSize, GraphicsDxD* pGfx);
	~UIImage();
	
	void Draw(GraphicsDxD* pGfx) const;

private:
	Image *cPImage;

	DirectX::XMFLOAT2 cSize;
	DirectX::XMFLOAT2 cPos;

};