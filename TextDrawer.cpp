#include "TextDrawer.h"

TextDrawer::TextDrawer(const Font& font, const DirectX::XMFLOAT2& defWndSize)
	:
	cDefWndSize(defWndSize),
	cFont(font)
{}

TextDrawer::~TextDrawer()
{}

void TextDrawer::DrawString(const std::string& text, const DirectX::XMFLOAT2& pos, float txtHeight, GraphicsDxD* pGfx) const
{
	const float wndWidth = static_cast<float>(pGfx->GetWndWidth());
	const float wndHeight = static_cast<float>(pGfx->GetWndHeight());

	const DirectX::XMFLOAT2 posicao = { (pos.x * wndWidth / cDefWndSize.x) - (wndWidth / 2.0f),
		(pos.y * wndHeight / cDefWndSize.y) - (wndHeight / 2.0f) };

	const DirectX::XMFLOAT2 dimL = cFont.GetLetterDim();
	const DirectX::XMFLOAT2 size = {(dimL.x * txtHeight / dimL.y) * wndWidth / cDefWndSize.x, (txtHeight) * wndHeight / cDefWndSize.y };

	cFont.Write(text, posicao, size, Font::LEFT, pGfx);
}