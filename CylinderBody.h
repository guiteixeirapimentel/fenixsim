#pragma once
#include "Cylinder.h"
#include "GraphicsDxd.h"
#include "Texture.h"

class CylinderBody
{
public:
	CylinderBody(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, const std::wstring& textureFileName, GraphicsDxD* pGfx);
	CylinderBody() = delete;
	CylinderBody(const CylinderBody&) = delete;
	~CylinderBody();

	void Render(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rotation, float scaleFactor,
		const DirectX::XMMATRIX& viewMatrix, GraphicsDxD* pGfx) const;

private:
	Texture* cPTexture;
	Cylinder* cPCylinder;
};