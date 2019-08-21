#pragma once
#include "Font.h"

class TextDrawer
{
public:
	TextDrawer(const Font& font, const DirectX::XMFLOAT2& defWndSize);
	~TextDrawer();

	void DrawString(const std::string& text, const DirectX::XMFLOAT2& pos, float txtHeight, GraphicsDxD* pGfx) const;
private:
	const Font& cFont;
	const DirectX::XMFLOAT2 cDefWndSize;
};