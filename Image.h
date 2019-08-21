#pragma once
#include "GraphicsDxd.h"
#include "Texture.h"
#include "Quad.h"

class Image
{
public:
	Image(const std::wstring& fileName, GraphicsDxD* pGfx);
	Image() = delete;
	Image(const Image& img) = delete;
	~Image();

	void Draw(const DirectX::XMFLOAT2& pos, float angle, const DirectX::XMMATRIX& viewMatrix,
		GraphicsDxD* pGfx) const;

	void Draw(const DirectX::XMFLOAT2& pos, float angle, const DirectX::XMFLOAT2& dimensions,
		const DirectX::XMMATRIX& viewMatrix, GraphicsDxD* pGfx) const;

	void Draw(const DirectX::XMFLOAT2& pos, float angle, float scaleFactor,
		const DirectX::XMMATRIX& viewMatrix, GraphicsDxD* pGfx) const;


private:
	Texture* cPTexture;
	const Quad* cPQuad;

	float cWidth;
	float cHeight;
};