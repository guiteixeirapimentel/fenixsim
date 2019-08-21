#include "Planet.h"

#include <DirectXMath.h>

Planet::Planet(float radius, UINT sliceCount, UINT stackCount, const std::wstring& fileName, GraphicsDxD* pGfx)
	:
	cPTexture(nullptr),
	cPSphere(nullptr)
{
	cPTexture = new Texture(fileName, pGfx->GetPDevice(), pGfx->GetPDeviceContext());

	cPSphere = new Sphere(radius, sliceCount, stackCount, pGfx->GetPDevice());

	cMaterial.cAmbient = { 1.0f, 1.0f,1.0f,1.0f };
	cMaterial.cDiffuse = { 1.0f, 1.0f,1.0f,1.0f };
	cMaterial.cReflect = { 1.0f,1.0f,1.0f,1.0f };
	cMaterial.cSpecular = { 1.0f, 1.0f, 1.0f,1.0f };
}

Planet::~Planet()
{
	if (cPSphere)
	{
		delete cPSphere;
		cPSphere = nullptr;
	}

	if (cPTexture)
	{
		delete cPTexture;
		cPTexture = nullptr;
	}
}
void Planet::Render(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rotation, float scaleFactor,
	const DirectX::XMMATRIX& viewMatrix, GraphicsDxD* pGfx) const
{
	ID3D11Buffer* pVertexBuffer = cPSphere->GetVertexBuffer();
	ID3D11Buffer* pIndexBuffer = cPSphere->GetIndexBuffer();

	DirectX::XMVECTOR v = { rotation.x, rotation.y, rotation.z, 1.0 };

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor) *
		DirectX::XMMatrixRotationRollPitchYawFromVector(v) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	pGfx->RenderModel3DLights(viewMatrix, worldMatrix, pGfx->GetProjMatrix(), cPTexture->GetTexture(), cMaterial, pVertexBuffer,
		pIndexBuffer, cPSphere->GetIndexCount());	
}
