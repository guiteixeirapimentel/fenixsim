#pragma once
#include "Sphere.h"
#include "GraphicsDxd.h"
#include "Texture.h"

class Planet
{
public:
	Planet(float radius, UINT sliceCount, UINT stackCount, const std::wstring& textureFileName, GraphicsDxD* pGfx);
	Planet() = delete;
	Planet(const Planet&) = delete;
	~Planet();

	void Render(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rotation, float scaleFactor,
		const DirectX::XMMATRIX& viewMatrix, GraphicsDxD* pGfx) const;

private:
	Texture* cPTexture;
	Sphere* cPSphere;

	Material cMaterial;
};