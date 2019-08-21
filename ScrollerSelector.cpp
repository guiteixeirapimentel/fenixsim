#include "ScrollerSelector.h"

ScrollerSelector::ScrollerSelector(const DirectX::XMFLOAT2& size, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& fontSize,
	const DirectX::XMFLOAT2& borders, float verticalTextSpacing,
	const std::wstring& bgImgPath, const ButtonDesc& descArrowUp, const ButtonDesc& descArrowDown,
	const Font& whiteFont, const Font& blackFont, const DirectX::XMFLOAT2& defWndSize, GraphicsDxD* pGfx)
	:
	cDefWndSize(defWndSize),
	cPos(pos),
	cSize(size),
	cWhiteFont(whiteFont),
	cBlackFont(blackFont),
	cFontSize(fontSize),
	cBackground(bgImgPath, size, pos, defWndSize, pGfx),
	cSelectedOptionID(0),
	cShownOffset(0),
	cButUp(descArrowUp, pGfx),
	cButDown(descArrowDown, pGfx),
	cVerticalTextSpacing(verticalTextSpacing),
	cBorders(borders)
{}

ScrollerSelector::~ScrollerSelector()
{}

void ScrollerSelector::Draw(const DirectX::XMFLOAT2& posMouse, GraphicsDxD* pGfx) const
{
	cBackground.Draw(pGfx);

	cButUp.Draw(posMouse, pGfx);
	cButDown.Draw(posMouse, pGfx);

	size_t maxIndexShown = min(cShownOffset + static_cast<size_t>((cSize.y / (cFontSize.y + cVerticalTextSpacing)) - 0.2f), cOptions.size());

	DirectX::XMFLOAT2 wndSize = { float(pGfx->GetWndWidth()), float(pGfx->GetWndHeight()) };

	DirectX::XMFLOAT2 pos;
	pos = { cPos.x + cBorders.x, cPos.y + cBorders.y };

	const DirectX::XMFLOAT2 corrFactor = { wndSize.x / cDefWndSize.x,wndSize.y / cDefWndSize.y };
	
	pos.x *= corrFactor.x;
	pos.y *= corrFactor.y;
	
	for (size_t i = (size_t)cShownOffset; i < maxIndexShown; i++)
	{	
		size_t ii = i - cShownOffset;
		DirectX::XMFLOAT2 posTxt = { 0.0f, (corrFactor.y * ii * cFontSize.y) + (corrFactor.y * cVerticalTextSpacing * ii) };
		
		if (i == cSelectedOptionID)
		{			
			cBlackFont.Write(cOptions[i].optionText, { posTxt.x + pos.x - (wndSize.x/2.0f), posTxt.y + pos.y - (wndSize.y / 2.0f) },
			{ cFontSize.x * corrFactor.x, cFontSize.y * corrFactor.y }, Font::LEFT, pGfx);
		}
		else
		{
			cWhiteFont.Write(cOptions[i].optionText, { posTxt.x + pos.x - (wndSize.x / 2.0f), posTxt.y + pos.y - (wndSize.y / 2.0f) },
			{ cFontSize.x * corrFactor.x, cFontSize.y * corrFactor.y }, Font::LEFT, pGfx);
		}
	}
}

bool ScrollerSelector::AddOption(const std::string& text)
{
	Option op(text);

	cOptions.push_back(op);

	int tmp = static_cast<int>((cSize.y / (cFontSize.y + cVerticalTextSpacing)) - 0.2f);

	cShownOffset = min(max(int(cOptions.size()) - tmp, 0), cShownOffset + 1);

	return true;
}

bool ScrollerSelector::DeleteOption(uint32_t id)
{
	size_t index = 0;
	std::vector<Option> newOptions;

	for (size_t i = 0; i < cOptions.size(); i++)
	{
		if (i != id)
		{
			newOptions.push_back(cOptions[i]);			
		}
	}
	
	cOptions = std::move(newOptions);

	cSelectedOptionID = 0;
	cShownOffset = max(0, cShownOffset - 1);
	
	return true;
}

uint32_t ScrollerSelector::GetSelectedOptionID() const
{
	return cSelectedOptionID;
}

std::string ScrollerSelector::GetSelectedOptionText() const
{
	return cOptions[cSelectedOptionID].optionText;

	return "ID INVALIDO";
}

void ScrollerSelector::MousePress(const DirectX::XMFLOAT2& posMouse, const GraphicsDxD* pGfx)
{
	cButUp.MousePress(posMouse, pGfx);
	cButDown.MousePress(posMouse, pGfx);

	int tmp = static_cast<int>((cSize.y / (cFontSize.y + cVerticalTextSpacing)) - 0.2f);

	if (cButUp.GetValue() > 0)
	{
		cShownOffset = min(max(int(cOptions.size()) - tmp, 0), cShownOffset + cButUp.GetValue());
		cButUp.SetValue(0);
	}
	if (cButDown.GetValue() > 0)
	{
		cShownOffset = max(0, cShownOffset - cButDown.GetValue());
		cButDown.SetValue(0);
	}
	
	size_t maxIndexShown = min(cShownOffset + static_cast<size_t>((cSize.y / (cFontSize.y + cVerticalTextSpacing)) - 0.2f), cOptions.size());

	DirectX::XMFLOAT2 wndSize = { float(pGfx->GetWndWidth()), float(pGfx->GetWndHeight()) };

	DirectX::XMFLOAT2 pos;
	pos = { cPos.x + cBorders.x, cPos.y + cBorders.y };

	const DirectX::XMFLOAT2 corrFactor = { wndSize.x / cDefWndSize.x,wndSize.y / cDefWndSize.y };

	pos.x *= corrFactor.x;
	pos.y *= corrFactor.y;

	for (size_t i = (size_t)cShownOffset; i < maxIndexShown; i++)
	{
		size_t ii = i - cShownOffset;
		DirectX::XMFLOAT2 posTxt = { 0.0f, (corrFactor.y * ii * cFontSize.y) + (corrFactor.y * cVerticalTextSpacing * ii) };

		DirectX::XMFLOAT2 topoEsqCaixa = {pos.x + posTxt.x, pos.y + posTxt.y };
		DirectX::XMFLOAT2 baixoDirCaixa = { pos.x + posTxt.x + (corrFactor.x * (cFontSize.x * cOptions[i].optionText.size())) ,
			pos.y + posTxt.y + (corrFactor.y * cFontSize.y) };

		if (posMouse.x > topoEsqCaixa.x && posMouse.y > topoEsqCaixa.y
			&& posMouse.x < baixoDirCaixa.x && posMouse.y < baixoDirCaixa.y)
		{
			cSelectedOptionID =	i;
		}
	}
}

void ScrollerSelector::MouseRelease(const DirectX::XMFLOAT2& posMouse, const GraphicsDxD* pGfx)
{
	cButUp.MouseRelease(posMouse, pGfx);
	cButDown.MouseRelease(posMouse, pGfx);
}

void ScrollerSelector::DeleteAllOptions()
{
	cOptions.clear();

	cSelectedOptionID = 0;
	cShownOffset = 0;
}