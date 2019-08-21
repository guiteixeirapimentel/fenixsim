#pragma once
#include <vector>
#include <string>

#include "AdderButton.h"
#include "TextBox.h"
#include "Font.h"

#include "UIImage.h"

class ScrollerSelector
{
public:
	class Option
	{
	public:
		Option(const std::string& optionText)
			:
			optionText(optionText)
		{}
		Option(const Option& opt)
			:
			optionText(opt.optionText)
		{}
		Option()
			:
			optionText("")
		{}
		const std::string optionText;
	};
public:
	ScrollerSelector(const DirectX::XMFLOAT2& size, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& fontSize,
		const DirectX::XMFLOAT2& borders, float verticalTextSpacing,
		const std::wstring& bgImgPath, const ButtonDesc& descArrowUp, const ButtonDesc& descArrowDown,
		const Font& whiteFont, const Font& blackFont, const DirectX::XMFLOAT2& defWndSize, GraphicsDxD* pGfx);
	~ScrollerSelector();
	
	void Draw(const DirectX::XMFLOAT2& posMouseAtual, GraphicsDxD* pGfx) const;

	bool AddOption(const std::string& text);
	bool DeleteOption(uint32_t id);

	void DeleteAllOptions();

	uint32_t GetSelectedOptionID() const;
	std::string GetSelectedOptionText() const;

	void MousePress(const DirectX::XMFLOAT2& posMouse, const GraphicsDxD* pGfx);
	void MouseRelease(const DirectX::XMFLOAT2& posMouse, const GraphicsDxD* pGfx);
	
private:
	const DirectX::XMFLOAT2 cDefWndSize;
	const DirectX::XMFLOAT2 cPos;
	const DirectX::XMFLOAT2 cSize;

	const DirectX::XMFLOAT2 cFontSize;
		
	const Font& cWhiteFont;
	const Font& cBlackFont;

	const UIImage cBackground;

	const float cVerticalTextSpacing;
	const DirectX::XMFLOAT2 cBorders;
	
	AdderButton cButUp;
	AdderButton cButDown;

	std::vector<Option> cOptions;

	uint32_t cSelectedOptionID;

	int cShownOffset;
};