#include "Font.h"
#include "GDIPlusManager.h"
#include <gdiplus.h>

Font::Font(const std::wstring& fontFile, const DirectX::XMFLOAT2& letterDim, GraphicsDxD* pGfx)
	:
	cPQuad(pGfx->GetPQuad()),
	cPPLetterTexture(nullptr),
	cNLetters(96),
	cLetterDimensions(letterDim)
{
	GDIPlusManager gdiMan;
	char* pBuffer = nullptr;

	Gdiplus::Bitmap bitmap(fontFile.c_str());
	if (bitmap.GetLastStatus() != Gdiplus::Status::Ok)
	{
		assert(0);
	}

	const unsigned int width = bitmap.GetWidth();
	const unsigned int pitch = (width * 4) * sizeof(unsigned char);
	const unsigned int height = bitmap.GetHeight();
	pBuffer = new char[height * width * 4];

	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			Gdiplus::Color c;
			bitmap.GetPixel(x, y, &c);
			reinterpret_cast<uint32_t*>(pBuffer)[(y * width) + x] = c.GetValue();
		}
	}

	cPPLetterTexture = new Texture*[96];

	const unsigned int letterWidth = (unsigned int)letterDim.x;
	const unsigned int letterHeight = (unsigned int)letterDim.y;

	char *pLetterBuffer = new char[letterWidth * letterHeight * 4];

	for (unsigned int i = 0; i < cNLetters; i++)
	{
		const int sheetIndex = i;
		const int sheetCol = sheetIndex % 32;
		const int sheetRow = sheetIndex / 32;
		const int xStart = sheetCol * letterWidth;
		const int yStart = sheetRow * letterHeight;
		const int xEnd = xStart + letterWidth;
		const int yEnd = yStart + letterHeight;
		const int surfWidth = width;


		//Make a copy of one letter worth of texture to the letter buffer
		for (int y = yStart; y < yEnd; y++)
		{
			for (int x = xStart; x < xEnd; x++)
			{
				reinterpret_cast<uint32_t*>(pLetterBuffer)[(x - xStart) + ((y - yStart)*letterWidth)] =
				reinterpret_cast<uint32_t*>(pBuffer)[x + (y * surfWidth)];
			}
		}
		

		cPPLetterTexture[i] = new Texture(pLetterBuffer, letterWidth, letterHeight, pGfx->GetPDevice(), 
			pGfx->GetPDeviceContext());
	}

	delete[] pLetterBuffer;
	pLetterBuffer = nullptr;

	delete[]pBuffer;
	pBuffer = nullptr;
}

Font::~Font()
{
	if (cPPLetterTexture)
	{
		for (unsigned int i = 0; i < cNLetters; i++)
		{
			delete cPPLetterTexture[i];
			cPPLetterTexture[i] = nullptr;
		}
		delete[] cPPLetterTexture;
		cPPLetterTexture = nullptr;
	}
}

void Font::WriteLetter(char c, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& size, float angle, GraphicsDxD* pGfx) const
{
	if (c - ' ' < 0)
	{
		//assert(0);
		return;
	}
	ID3D11Buffer* pVertexBuffer = cPQuad->GetVertexBuffer();
	ID3D11Buffer* pIndexBuffer = cPQuad->GetIndexBuffer();

	DirectX::XMFLOAT2 finalPos = { pos.x, -pos.y };

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixScaling(size.x, size.y, 0.0f) *
		DirectX::XMMatrixRotationZ(angle) *
		DirectX::XMMatrixTranslation(finalPos.x, finalPos.y, 0.0f);

	static DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixIdentity();

	pGfx->RenderModel2D(viewMatrix, worldMatrix, pGfx->GetOrthoMatrix(), cPPLetterTexture[c-' ']->GetTexture(), pVertexBuffer,
		pIndexBuffer, cPQuad->GetIndexCount());
}
void Font::WriteLetter(char c, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& size, GraphicsDxD* pGfx) const
{
	WriteLetter(c, pos, size, 0.0f, pGfx);
}

void Font::Write(const std::string& text, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& size, float angle, ALIGN align, GraphicsDxD* pGfx) const
{
	switch (align)
	{
	case RIGHT:
		for (size_t i = 0; i < text.size(); i++)
		{
			char c = text.c_str()[i];
			//assert((c - ' ') >= 0);

			WriteLetter(c, { pos.x - (((int64_t)text.size() - (int64_t)i)*size.x), pos.y + (size.y / 2.0f) }, size, angle, pGfx);
		}
		break;
	case LEFT:
		for (size_t i = 0; i < text.size(); i++)
		{
			char c = text.c_str()[i];
			//assert((c - ' ') >= 0);

			WriteLetter(c, { pos.x + (i*size.x) +(size.x/2.0f), pos.y + (size.y / 2.0f) }, size, angle, pGfx);
		}
		break;
	case CENTER:
		for (size_t i = 0; i < text.size(); i++)
		{
			char c = text.c_str()[i];
			//assert((c - ' ') >= 0);

			WriteLetter(c, { pos.x + (i*size.x) - (text.size() * 0.5f * size.x), pos.y + (size.y / 2.0f) }, size, angle, pGfx);
		}
		break;
	}	
}
void Font::Write(const std::string& text, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& size, ALIGN align, GraphicsDxD* pGfx) const
{
	Write(text, pos, size, 0.0f, align, pGfx);
}