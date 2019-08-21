#pragma once
#include "GraphicsDxd.h"
#include "Texture.h"
#include "Quad.h"

#include <string>
#include <fstream>

class Font
{
public:
	enum ALIGN
	{
		LEFT,
		RIGHT,
		CENTER
	};

public:
	Font(const std::wstring& fontFile, const DirectX::XMFLOAT2& letterDim, GraphicsDxD* pGfx);
	~Font();

	void Write(const std::string& text, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& size, float angle, ALIGN align, GraphicsDxD* pGfx) const;
	void Write(const std::string& text, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& size, ALIGN align, GraphicsDxD* pGfx) const;

	DirectX::XMFLOAT2 GetLetterDim() const
	{
		return cLetterDimensions;
	}

private:
	void WriteLetter(char c, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& size, float angle, GraphicsDxD* pGfx) const;
	void WriteLetter(char c, const DirectX::XMFLOAT2& pos, const DirectX::XMFLOAT2& size, GraphicsDxD* pGfx) const;

private:
	const Quad* cPQuad;
	Texture** cPPLetterTexture;

	const unsigned int cNLetters;

	const DirectX::XMFLOAT2 cLetterDimensions;
};