#include "CylinderBody.h"

#include <DirectXMath.h>

CylinderBody::CylinderBody(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, const std::wstring& fileName, GraphicsDxD* pGfx)
	:
	cPTexture(nullptr),
	cPCylinder(nullptr)
{
	cPTexture = new Texture(fileName, pGfx->GetPDevice(), pGfx->GetPDeviceContext());

	cPCylinder = new Cylinder(bottomRadius, topRadius, height, sliceCount, stackCount, pGfx->GetPDevice());
}

CylinderBody::~CylinderBody()
{
	if (cPCylinder)
	{
		delete cPCylinder;
		cPCylinder = nullptr;
	}

	if (cPTexture)
	{
		delete cPTexture;
		cPTexture = nullptr;
	}
}
void CylinderBody::Render(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rotation, float scaleFactor,
	const DirectX::XMMATRIX& viewMatrix, GraphicsDxD* pGfx) const
{
	ID3D11Buffer* pVertexBuffer = cPCylinder->GetVertexBuffer();
	ID3D11Buffer* pIndexBuffer = cPCylinder->GetIndexBuffer();

	DirectX::XMVECTOR v = { rotation.x, rotation.y, rotation.z, 1.0 };

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor) *
		DirectX::XMMatrixRotationRollPitchYawFromVector(v) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	pGfx->RenderModel3D(viewMatrix, worldMatrix, pGfx->GetProjMatrix(), cPTexture->GetTexture(), pVertexBuffer,
		pIndexBuffer, cPCylinder->GetIndexCount());
}
