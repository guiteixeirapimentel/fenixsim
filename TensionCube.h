#pragma once
#include "Cube.h"
#include "GraphicsDxd.h"
#include "Texture.h"

class TensionCube
{
public:
	TensionCube(const std::wstring& fileName, GraphicsDxD* pGfx);
	TensionCube() = delete;
	TensionCube(const TensionCube&) = delete;
	~TensionCube();

	void Render(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rotation, float scaleFactor, 
		const DirectX::XMMATRIX& viewMatrix, GraphicsDxD* pGfx) const;

private:
	Texture* cPTexture;
	const Cube* cPCube;
	Material cMaterial;
};