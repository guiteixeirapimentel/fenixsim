#include "RocketModel.h"
#include "RocketSim.h"

constexpr float PI = 3.14159265359f;

RocketModel::RocketModel(const RocketData& rd, GraphicsDxD* pGfx)
	:
	cBodyModel((float)rd.GetData().cRaioCorpo, (float)rd.GetData().cRaioCorpo, (float)rd.GetData().cAlturaCorpo, 
		100, 100, pGfx->GetPDevice()),
	cNoseCoseModel((float)rd.GetData().cRaioBaseCoifa, 0.0f, (float)rd.GetData().cAlturaCoifa, 100, 100, pGfx->GetPDevice()),
	cFinModel((float)rd.GetData().cCordaRaizAleta, (float)rd.GetData().cCordaPontaAleta, 
	(float)rd.GetData().cDeslocamentoCordaRaizAletas, (float)rd.GetData().cEnvergaduraAletas,
	(float)rd.GetData().cEspessuraAletas, pGfx->GetPDevice()),
	cBodyTexture(L"imgs/textCylinder.png", pGfx->GetPDevice(), pGfx->GetPDeviceContext()),
	cNoseConeTexture(L"imgs/noseConeTex.png", pGfx->GetPDevice(), pGfx->GetPDeviceContext()),
	cFinTexture(L"imgs/finText.png", pGfx->GetPDevice(), pGfx->GetPDeviceContext()),
	cAlturaCorpo((float)rd.GetData().cAlturaCorpo),
	cAlturaNose((float)rd.GetData().cAlturaCoifa),
	cRaioCorpo((float)rd.GetData().cRaioCorpo),
	cNumFins(rd.GetData().cNumeroAletas),
	cCordaRaizAleta(float(rd.GetData().cCordaRaizAleta))
{}

RocketModel::~RocketModel()
{}

void RocketModel::Raster(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& ati, 
	const DirectX::XMMATRIX& viewMatrix, GraphicsDxD* cPGraphics) const
{
	DirectX::XMVECTOR vUnit = { ati.x, ati.y, ati.z };
	DirectX::XMVECTOR vModel = { 0.0f, 1.0f, 0.0f };
	double angleRot = 0.0;
	DirectX::XMVECTOR axis = vModel;

	if (!DirectX::XMVector3EqualInt(vUnit, vModel))
	{
		angleRot = acosf(DirectX::XMVector3Dot(vModel, vUnit).m128_f32[0]);
		axis = DirectX::XMVector3Cross(vModel, vUnit);
	}

	//Render BODY MODEL
	ID3D11Buffer* pVertexBuffer = cBodyModel.GetVertexBuffer();
	ID3D11Buffer* pIndexBuffer = cBodyModel.GetIndexBuffer();

	DirectX::XMVECTOR v = { (float)ati.x, (float)ati.y, (float)ati.z, (float)1.0 };

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixRotationNormal(axis, (float)angleRot) *
		DirectX::XMMatrixTranslation((float)pos.x, (float)pos.y, (float)pos.z);

	cPGraphics->RenderModel3D(viewMatrix, worldMatrix, cPGraphics->GetProjMatrix(), cBodyTexture.GetTexture(), pVertexBuffer,
		pIndexBuffer, cBodyModel.GetIndexCount());

	//RENDER NOSE CONE MODEL
	pVertexBuffer = cNoseCoseModel.GetVertexBuffer();
	pIndexBuffer = cNoseCoseModel.GetIndexBuffer();

	worldMatrix = DirectX::XMMatrixTranslation(0.0f, (cAlturaCorpo + cAlturaNose) / 2.0f, 0.0f) *
		DirectX::XMMatrixRotationNormal(axis, (float)angleRot) *
		DirectX::XMMatrixTranslation((float)pos.x, (float)pos.y, (float)pos.z);

	cPGraphics->RenderModel3D(viewMatrix, worldMatrix, cPGraphics->GetProjMatrix(), cNoseConeTexture.GetTexture(),
		pVertexBuffer, pIndexBuffer, cNoseCoseModel.GetIndexCount());

	//RENDER FINS

	const float passo = (2.0f * (float)PI) / static_cast<float>(cNumFins);
	for (unsigned int i = 0; i < cNumFins; i++)
	{
		pVertexBuffer = cFinModel.GetVertexBuffer();
		pIndexBuffer = cFinModel.GetIndexBuffer();

		DirectX::XMMATRIX adjust = DirectX::XMMatrixRotationY(static_cast<float>(i) * passo)
			* DirectX::XMMatrixTranslation(cRaioCorpo * cosf(static_cast<float>(i) * passo),
				-float(cAlturaCorpo - cCordaRaizAleta) / 2.0f, (float)-cRaioCorpo * sinf(static_cast<float>(i) * passo));

		worldMatrix = adjust * DirectX::XMMatrixRotationNormal(axis, (float)angleRot) *
			DirectX::XMMatrixTranslation((float)pos.x, (float)pos.y, (float)pos.z);

		cPGraphics->RenderModel3D(viewMatrix, worldMatrix, cPGraphics->GetProjMatrix(), cFinTexture.GetTexture(), pVertexBuffer,
			pIndexBuffer, cFinModel.GetIndexCount());
	}
}