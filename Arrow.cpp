#include "Arrow.h"
#include <DirectXMath.h>

Arrow::Arrow(const std::wstring& fileName, GraphicsDxD* pGfx)
	:
	cPTexture(nullptr),
	cPBody(nullptr),
	cPTip(nullptr)
{
	cPTexture = new Texture(fileName, pGfx->GetPDevice(), pGfx->GetPDeviceContext());

	cPBody = new Cylinder(0.025f, 0.025f, 1.0f, 30, 30, pGfx->GetPDevice());
	cPTip = new Cylinder(0.045f, 0.0f, 0.08f, 30, 30, pGfx->GetPDevice());
}

Arrow::~Arrow()
{
	if (cPBody)
	{
		delete cPBody;
		cPBody = nullptr;
	}
	if (cPTip)
	{
		delete cPTip;
		cPTip = nullptr;
	}
	if (cPTexture)
	{
		delete cPTexture;
		cPTexture = nullptr;
	}
}
void Arrow::Render(const VETOR& pos, const VETOR& vetor,
	const DirectX::XMMATRIX& viewMatrix, GraphicsDxD* pGfx) const
{	
	float scaleFactor = (float)Modulus(vetor);
	DirectX::XMVECTOR vUnit = { float(vetor.x / scaleFactor), float(vetor.y / scaleFactor), float(vetor.z / scaleFactor) };
	DirectX::XMVECTOR vModel = { 0.0f, 1.0f, 0.0f };
	double angleRot = 0.0;
	DirectX::XMVECTOR axis = vModel;

	if (!DirectX::XMVector3EqualInt(vUnit, vModel))
	{
		angleRot = acosf(DirectX::XMVector3Dot(vModel, vUnit).m128_f32[0]);
		axis = DirectX::XMVector3Cross(vModel, vUnit);
	}		
	
	if (DirectX::XMVector3Equal(axis, DirectX::XMVectorZero()))
	{
		axis.m128_f32[0] = 1.0;
	}
	//CORPO

	ID3D11Buffer* pVertexBuffer = cPBody->GetVertexBuffer();
	ID3D11Buffer* pIndexBuffer = cPBody->GetIndexBuffer();
	
	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixTranslation(0.0f, 0.5f, 0.0f)
	* DirectX::XMMatrixScaling(1.0f * scaleFactor / 1.0f, (float)scaleFactor - 0.08f, 1.0f * scaleFactor / 1.0f) *
		DirectX::XMMatrixRotationAxis(axis, (float)angleRot) *
		DirectX::XMMatrixTranslation((float)pos.x, (float)pos.y, (float)pos.z);

	pGfx->RenderModel3D(viewMatrix, worldMatrix, pGfx->GetProjMatrix(), cPTexture->GetTexture(), pVertexBuffer,
		pIndexBuffer, cPBody->GetIndexCount());

	//PONTA

	pVertexBuffer = cPTip->GetVertexBuffer();
	pIndexBuffer = cPTip->GetIndexBuffer();

	worldMatrix = DirectX::XMMatrixTranslation(0.0f, (1.0f * (float)scaleFactor) - 0.04f, 0.0f)*
		DirectX::XMMatrixScaling(1.0f * scaleFactor / 1.0f, 1.0f, 1.0f * scaleFactor / 1.0f) *
		DirectX::XMMatrixRotationAxis(axis, (float)angleRot) *
		DirectX::XMMatrixTranslation((float)pos.x, (float)pos.y, (float)pos.z);

	pGfx->RenderModel3D(viewMatrix, worldMatrix, pGfx->GetProjMatrix(), cPTexture->GetTexture(), pVertexBuffer,
		pIndexBuffer, cPTip->GetIndexCount());
}
