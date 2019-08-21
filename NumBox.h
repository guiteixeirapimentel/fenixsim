#pragma once
#include "TextBox.h"

class NumBox : public TextBox
{
public:
	NumBox(const BoxDesc& boxDesc, GraphicsDxD* pGfx);

	void KeyPress(unsigned char key) override;

	void ClearZeros()
	{
		cText.erase(cText.find_last_not_of('0') + 1, std::string::npos);
		if (cText.back() == '.')
			cText.push_back('0');
	}
};