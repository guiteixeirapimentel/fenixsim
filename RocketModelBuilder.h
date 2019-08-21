#pragma once
#include "GraphicsDxd.h"
#include "Texture.h"
#include "Cylinder.h"
#include "FinModelDynamic.h"
#include "Foguete.h"

class RocketModelBuilder
{
private:
	class TransicaoModel
	{
	public:
		TransicaoModel(float raioTopo, float raioBase, float altura, UINT sliceCount, UINT stackCount,
			uint32_t id, ID3D11Device* pDevice)
			:
			cPModelTransicao(new Cylinder(raioBase, raioTopo, altura, sliceCount, stackCount, pDevice)),
			cId(id)
		{}
		TransicaoModel()
			:			
			cPModelTransicao(nullptr),
			cId(0)
		{}
		~TransicaoModel()
		{
			if (cPModelTransicao)
			{
				delete cPModelTransicao;
				cPModelTransicao = nullptr;
			}
		}

		Cylinder* GetPtrModel()
		{
			return cPModelTransicao;
		}
		uint32_t GetId()
		{
			return cId;
		}

		void UpdateTransicao(float altura, float raioBase, float raioTopo, GraphicsDxD* pGfx);
	private:
		Cylinder *cPModelTransicao;
		uint32_t cId;
	};
public:
	RocketModelBuilder(float cordaRaiz, float cordaPonta, float deslCorda, float envergadura, float espessura, float deslBaseFoguete, GraphicsDxD* pGfx);
	~RocketModelBuilder();

	void Raster(const Foguete& fogueteData, float zoom,
		float rot, const DirectX::XMFLOAT3& ati, const DirectX::XMMATRIX& viewMatrix, GraphicsDxD* cPGraphics) const;

	void SetAletaInfo(float cordaRaiz, float cordaPonta, float deslCorda, float envergadura,
		float espessura, GraphicsDxD* pGfx)
	{
		if (cCordaRaiz != cordaRaiz || cCordaPonta != cordaPonta || cDeslCorda != deslCorda
			|| cEnvergadura != envergadura || cEspessura != espessura)
		{
			cFinModel.ChangeFin(cordaRaiz, cordaPonta, deslCorda, envergadura, espessura, pGfx);
		}
		cCordaRaiz = cordaRaiz;
		cCordaPonta = cordaPonta;
		cDeslCorda = deslCorda;
		cEnvergadura = envergadura;
		cEspessura = espessura;
	}

	void UpdateTransicao(uint32_t id, float altura, float raioBase, float raioTopo, GraphicsDxD* pGfx);
	void AdicionarTransicao(float altura, float raioBase, float raioTopo, uint32_t id, GraphicsDxD* pGfx);
	void DeletarTransicao(uint32_t id);

	void DeletarTodasTransicoes();

private:
	TransicaoModel* GetPTransicao(uint32_t id) const;

private:
	float cCordaRaiz;
	float cCordaPonta;
	float cDeslCorda;
	float cEnvergadura;
	float cEspessura;
	
private:
	Cylinder cTuboModel;
	Cylinder cCoifaCilindricaModel;
	std::vector<TransicaoModel*> cTransicoes;
	FinModelDynamic cFinModel;

	Material cMaterial;

	Texture cBodyTexture;
	Texture cNoseConeTexture;
	Texture cFinTexture;
};