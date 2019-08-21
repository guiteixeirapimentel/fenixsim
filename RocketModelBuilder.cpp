#include "RocketModelBuilder.h"

constexpr float PI = 3.14159265359f;

RocketModelBuilder::RocketModelBuilder(float cordaRaiz, float cordaPonta, float deslCorda, float envergadura, float espessura, float deslBaseFoguete, GraphicsDxD* pGfx)
	:
	cTuboModel(1.0f, 1.0f, 1.0f, 100, 100, pGfx->GetPDevice()),
	cCoifaCilindricaModel(1.0f, 0.0f, 1.0f, 100, 100, pGfx->GetPDevice()),
	cFinModel(cordaRaiz, cordaPonta, deslCorda, envergadura,
		espessura, pGfx->GetPDevice()),
	cBodyTexture(L"imgs/textCylinder.png", pGfx->GetPDevice(), pGfx->GetPDeviceContext()),
	cNoseConeTexture(L"imgs/noseConeTex.png", pGfx->GetPDevice(), pGfx->GetPDeviceContext()),
	cFinTexture(L"imgs/finText.png", pGfx->GetPDevice(), pGfx->GetPDeviceContext())
{
	cMaterial.cAmbient = { 1.0f, 1.0f,1.0f,1.0f };
	cMaterial.cDiffuse = { 1.0f, 1.0f,1.0f,1.0f };
	cMaterial.cReflect = { 1.0f,1.0f,1.0f,1.0f };
	cMaterial.cSpecular = { 1.0f, 1.0f, 1.0f, 100000.0f };
}

RocketModelBuilder::~RocketModelBuilder()
{
	for (size_t i = 0; i < cTransicoes.size(); i++)
	{
		if (cTransicoes[i] != nullptr)
		{
			delete cTransicoes[i];
			cTransicoes[i] = nullptr;
		}
	}

	cTransicoes.clear();
}

void RocketModelBuilder::Raster(const Foguete& fogueteData, float zoom,
	float rot, const DirectX::XMFLOAT3& ati, const DirectX::XMMATRIX& viewMatrix, GraphicsDxD* cPGraphics) const
{
	//DESENHAR FOGUETE A PARTIR DA PONTA DA COIFA.
	DirectX::XMVECTOR atitude = DirectX::XMLoadFloat3(&ati);
	atitude = DirectX::XMVector3Normalize(atitude);

	DirectX::XMVECTOR vModel = { 0.0f, 1.0f, 0.0f };
	ID3D11Buffer* pVertexBuffer = nullptr;
	ID3D11Buffer* pIndexBuffer = nullptr;

	DirectX::XMVECTOR angle = DirectX::XMVector3AngleBetweenVectors(vModel, atitude);
	DirectX::XMVECTOR axis = DirectX::XMVector3Cross(vModel, atitude);

	if (angle.m128_f32[0] == 0.0)
	{
		axis = vModel;
	}

	DirectX::XMMATRIX worldMatrix;

	//Render BODY MODEL
	float altura = (float)fogueteData.cPartesCorpo[0]->PegarAltura();

	for (size_t i = 1; i < fogueteData.cPartesCorpo.size(); i++)
	{
		if (fogueteData.cTipoPartes[i] == Foguete::TUBO)
		{
			pVertexBuffer = cTuboModel.GetVertexBuffer();
			pIndexBuffer = cTuboModel.GetIndexBuffer();

			DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixScaling(1.0f + zoom, 1.0f + zoom, 1.0f + zoom) * 
				DirectX::XMMatrixScaling((float)fogueteData.cPartesCorpo[i]->PegarRaioBaseExt(), (float)fogueteData.cPartesCorpo[i]->PegarAltura(), (float)fogueteData.cPartesCorpo[i]->PegarRaioBaseExt()) *
				DirectX::XMMatrixRotationNormal(vModel, rot)*
				DirectX::XMMatrixTranslation(0.0f, (float(-fogueteData.cPartesCorpo[i]->PegarAltura()/2.0f) - altura)* (1.0f + zoom), 0.0f)
				* DirectX::XMMatrixRotationAxis(axis, angle.m128_f32[0]);

			cPGraphics->RenderModel3DLights(viewMatrix, worldMatrix, cPGraphics->GetProjMatrix(), cBodyTexture.GetTexture(),
				cMaterial, pVertexBuffer, pIndexBuffer, cTuboModel.GetIndexCount());			
		}
		else if (fogueteData.cTipoPartes[i] == Foguete::TRANSICAOAUMENTO ||
			fogueteData.cTipoPartes[i] == Foguete::TRANSICAODIMINUI)
		{
			TransicaoModel* pTrans = GetPTransicao(i);

			pVertexBuffer = pTrans->GetPtrModel()->GetVertexBuffer();
			pIndexBuffer = pTrans->GetPtrModel()->GetIndexBuffer();

			DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixScaling(1.0f + zoom, 1.0f + zoom, 1.0f + zoom) *				
				DirectX::XMMatrixRotationNormal(vModel, rot)*
				DirectX::XMMatrixTranslation(0.0f, (float(-fogueteData.cPartesCorpo[i]->PegarAltura() / 2.0f) - altura) * (1.0f + zoom), 0.0f)
				* DirectX::XMMatrixRotationAxis(axis, angle.m128_f32[0]);

			cPGraphics->RenderModel3DLights(viewMatrix, worldMatrix, cPGraphics->GetProjMatrix(), cBodyTexture.GetTexture(),
				cMaterial, pVertexBuffer, pIndexBuffer, pTrans->GetPtrModel()->GetIndexCount());
		}

		altura += (float)fogueteData.cPartesCorpo[i]->PegarAltura();
	}
		

	//RENDER NOSE CONE MODEL
	pVertexBuffer = cCoifaCilindricaModel.GetVertexBuffer();
	pIndexBuffer = cCoifaCilindricaModel.GetIndexBuffer();

	worldMatrix = DirectX::XMMatrixScaling(1.0f + zoom, 1.0f + zoom, 1.0f + zoom) * DirectX::XMMatrixRotationNormal(vModel, rot) * 
		DirectX::XMMatrixScaling((float)fogueteData.cPartesCorpo[0]->PegarRaioBaseExt(), (float)fogueteData.cPartesCorpo[0]->PegarAltura(), (float)fogueteData.cPartesCorpo[0]->PegarRaioBaseExt())
		*  DirectX::XMMatrixTranslation(0.0f, float(-fogueteData.cPartesCorpo[0]->PegarAltura() / 2.0f), 0.0f)
		* DirectX::XMMatrixRotationAxis(axis, angle.m128_f32[0]);

	cPGraphics->RenderModel3DLights(viewMatrix, worldMatrix, cPGraphics->GetProjMatrix(), cNoseConeTexture.GetTexture(),
		cMaterial,pVertexBuffer, pIndexBuffer, cCoifaCilindricaModel.GetIndexCount());

	//RENDER FINS

	const float passo = (2.0f * (float)PI) / static_cast<float>(fogueteData.cAletas[0]->PegarNumAletas());
	for (int i = 0; i < fogueteData.cAletas[0]->PegarNumAletas(); i++)
	{
		pVertexBuffer = cFinModel.GetVertexBuffer();
		pIndexBuffer = cFinModel.GetIndexBuffer();

		DirectX::XMMATRIX adjust = DirectX::XMMatrixRotationY(static_cast<float>(i) * passo)
			* DirectX::XMMatrixTranslation((float)fogueteData.cAletas[0]->PegarRaioNaAleta() * cosf(static_cast<float>(i) * passo),
				-(float)fogueteData.cAletas[0]->PegarPosV() + (float)(fogueteData.cAletas[0]->PegarCordaRaiz()/2.0f), (float)-fogueteData.cAletas[0]->PegarRaioNaAleta() * sinf(static_cast<float>(i) * passo));

		worldMatrix = adjust * DirectX::XMMatrixRotationNormal(vModel, rot) * DirectX::XMMatrixScaling(1.0f + zoom, 1.0f + zoom, 1.0f + zoom)
			* DirectX::XMMatrixRotationAxis(axis, angle.m128_f32[0]);

		cPGraphics->RenderModel3DLights(viewMatrix, worldMatrix, cPGraphics->GetProjMatrix(), cFinTexture.GetTexture(), 
			cMaterial, pVertexBuffer,pIndexBuffer, cFinModel.GetIndexCount());
	}
}

void RocketModelBuilder::TransicaoModel::UpdateTransicao(float altura, float raioBase, float raioTopo, GraphicsDxD* pGfx)
{
	if (cPModelTransicao)
	{
		delete cPModelTransicao;
		cPModelTransicao = nullptr;
	}

	cPModelTransicao = new Cylinder(raioBase, raioTopo, altura, 50, 50, pGfx->GetPDevice());
}

void RocketModelBuilder::UpdateTransicao(uint32_t id, float altura, float raioBase, float raioTopo, GraphicsDxD* pGfx)
{
	for (size_t i = 0; i < cTransicoes.size(); i++)
	{
		if (cTransicoes[i]->GetId() == id)
		{
			cTransicoes[i]->UpdateTransicao(altura, raioBase, raioTopo, pGfx);
			break;
		}
	}
}

void RocketModelBuilder::AdicionarTransicao(float altura, float raioBase, float raioTopo, uint32_t id, GraphicsDxD* pGfx)
{
	cTransicoes.push_back(new TransicaoModel(raioTopo, raioBase, altura, 50, 50, id, pGfx->GetPDevice()));
}

void RocketModelBuilder::DeletarTransicao(uint32_t id)
{
	std::vector<TransicaoModel*> newTransicoes;
	for (size_t i = 0; i < cTransicoes.size(); i++)
	{
		if (cTransicoes[i]->GetId() != id)
		{
			newTransicoes.push_back(cTransicoes[i]);
		}
		else
		{
			delete cTransicoes[i];
			cTransicoes[i] = nullptr;
		}
	}

	cTransicoes = newTransicoes;
}

void RocketModelBuilder::DeletarTodasTransicoes()
{
	for (size_t i = 0; i < cTransicoes.size(); i++)
	{
		if (cTransicoes[i] != nullptr)
		{
			delete cTransicoes[i];
			cTransicoes[i] = nullptr;
		}
	}

	cTransicoes.clear();
}

RocketModelBuilder::TransicaoModel* RocketModelBuilder::GetPTransicao(uint32_t id) const
{
	for (size_t i = 0; i < cTransicoes.size(); i++)
	{
		if (cTransicoes[i]->GetId() == id)
		{
			return cTransicoes[i];
		}
	}

	return nullptr;
}