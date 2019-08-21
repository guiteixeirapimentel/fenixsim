#include "UIImage.h"

UIImage::UIImage(const std::wstring& fileName, const DirectX::XMFLOAT2& size, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& defWndSize, GraphicsDxD* pGfx)
	:
	cPImage(nullptr),
	cPos({ pos.x / defWndSize.x, pos.y / defWndSize.y }),
	cSize({ size.x / defWndSize.x, size.y / defWndSize.y })
{
	cPImage = new Image(fileName, pGfx);
}

UIImage::~UIImage()
{
	if (cPImage)
	{
		delete cPImage;
		cPImage = nullptr;
	}
}

void UIImage::Draw(GraphicsDxD* pGfx) const
{
	const float wndWidth = static_cast<float>(pGfx->GetWndWidth());
	const float wndHeight = static_cast<float>(pGfx->GetWndHeight());

	const DirectX::XMFLOAT2 pos = { (cPos.x * wndWidth) - (wndWidth / 2.0f) + (cSize.x*wndWidth / 2.0f),
		(cPos.y * wndHeight) - (wndHeight / 2.0f) + (cSize.y * wndHeight / 2.0f) };

	cPImage->Draw(pos, 0.0f,
	{ (cSize.x * wndWidth), (cSize.y * wndHeight) }, DirectX::XMMatrixIdentity(), pGfx);

}