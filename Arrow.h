#pragma once
#include "Cylinder.h"
#include "GraphicsDxd.h"
#include "Texture.h"
#include "LinAlg.h"

class Arrow
{
public:
	Arrow(const std::wstring& fileName, GraphicsDxD* pGfx);
	Arrow() = delete;
	Arrow(const Cylinder&) = delete;
	~Arrow();

	void Render(const VETOR& pos, const VETOR& vector,
		const DirectX::XMMATRIX& viewMatrix, GraphicsDxD* pGfx) const;

private:
	Texture* cPTexture;
	Cylinder* cPBody;
	Cylinder* cPTip;
};