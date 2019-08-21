#include "TensionCube.h"

#include <DirectXMath.h>

TensionCube::TensionCube(const std::wstring& fileName, GraphicsDxD* pGfx)
	:
	cPTexture(nullptr),
	cPCube(pGfx->GetPCube())
{
	cPTexture = new Texture(fileName, pGfx->GetPDevice(), pGfx->GetPDeviceContext());
	cMaterial.cAmbient = { 1.0f, 1.0f,1.0f,1.0f };
	cMaterial.cDiffuse = {};//{ 1.0f, 1.0f,1.0f,1.0f };
	cMaterial.cReflect = {};//{ 1.0f,1.0f,1.0f,1.0f };
	cMaterial.cSpecular = {};//{ 1.0f, 1.0f,1.0f,1.0f };
}

TensionCube::~TensionCube()
{
	if (cPTexture)
	{
		delete cPTexture;
		cPTexture = nullptr;
	}
}
void TensionCube::Render(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& rotation, float scaleFactor,
	const DirectX::XMMATRIX& viewMatrix, GraphicsDxD* pGfx) const
{
	ID3D11Buffer* pVertexBuffer = cPCube->GetVertexBuffer();
	ID3D11Buffer* pIndexBuffer = cPCube->GetIndexBuffer();
	
	DirectX::XMVECTOR v = { rotation.x, rotation.y, rotation.z, 1.0 };

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixScaling(scaleFactor, scaleFactor, scaleFactor) *
		DirectX::XMMatrixRotationRollPitchYawFromVector(v) *
		DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	pGfx->RenderModel3DLights(viewMatrix, worldMatrix, pGfx->GetProjMatrix(), cPTexture->GetTexture(), cMaterial, pVertexBuffer,
		pIndexBuffer, cPCube->GetIndexCount());
}
