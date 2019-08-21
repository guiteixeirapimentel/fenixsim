#pragma once
#include "GraphicsDxd.h"
#include "Texture.h"
#include "Cylinder.h"
#include "FinModel.h"
#include "RocketData.h"

class RocketModel
{
public:
	RocketModel(const class RocketData& rocketDesc, GraphicsDxD* pGfx);
	~RocketModel();

	void Raster(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& atitude, 
		const DirectX::XMMATRIX& viewMatrix, GraphicsDxD* cPGraphics) const;
private:
	const unsigned int cNumFins;
	const float cCordaRaizAleta;
	const float cAlturaCorpo;
	const float cAlturaNose;
	const float cRaioCorpo;
private:
	Cylinder cBodyModel;
	Cylinder cNoseCoseModel;
	FinModel cFinModel;

	Texture cBodyTexture;
	Texture cNoseConeTexture;
	Texture cFinTexture;
};