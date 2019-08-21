#include "NumBox.h"

NumBox::NumBox(const BoxDesc& boxDesc, GraphicsDxD* pGfx)
	:
	TextBox(boxDesc, pGfx)
{
	cText = "0.0";
}

void NumBox::KeyPress(unsigned char c)
{
	if (cSelected)
	{
		if ((c >= 48 && c <= 57) || c == 46 || c == '+' || c == '-')
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
			cNewText = false;
			cText.pop_back();
		}
		else if (c == VK_RETURN)
		{
			cSelected = false;
		}
	}
}