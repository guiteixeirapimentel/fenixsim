#include "Image.h"

#include <DirectXMath.h>

Image::Image(const std::wstring& fileName, GraphicsDxD* pGfx)
	:
	cPTexture(nullptr),
	cPQuad(pGfx->GetPQuad())
{
	cPTexture = new Texture(fileName, pGfx->GetPDevice(), pGfx->GetPDeviceContext());

	cWidth = (float)cPTexture->GetWidth();
	cHeight = (float)cPTexture->GetHeight();
}

Image::~Image()
{
	if (cPTexture)
	{
		delete cPTexture;
		cPTexture = nullptr;
	}
}
void Image::Draw(const DirectX::XMFLOAT2& pos, float angle, const DirectX::XMMATRIX& viewMatrix,
	GraphicsDxD* pGfx) const
{
	ID3D11Buffer* pVertexBuffer = cPQuad->GetVertexBuffer();
	ID3D11Buffer* pIndexBuffer = cPQuad->GetIndexBuffer();

	DirectX::XMFLOAT2 finalPos = { pos.x, -pos.y };

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixScaling(cWidth, cHeight, 0.0f) *
		DirectX::XMMatrixRotationZ(angle) *
		DirectX::XMMatrixTranslation(finalPos.x, finalPos.y, 0.0f);
	
	pGfx->RenderModel2D(viewMatrix, worldMatrix, pGfx->GetOrthoMatrix(), cPTexture->GetTexture(), pVertexBuffer,
		pIndexBuffer, cPQuad->GetIndexCount());
}

void Image::Draw(const DirectX::XMFLOAT2& pos, float angle, const DirectX::XMFLOAT2& dimensions,
	const DirectX::XMMATRIX& viewMatrix, GraphicsDxD* pGfx) const
{
	ID3D11Buffer* pVertexBuffer = cPQuad->GetVertexBuffer();
	ID3D11Buffer* pIndexBuffer = cPQuad->GetIndexBuffer();

	DirectX::XMFLOAT2 finalPos = { pos.x, -pos.y };

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixScaling(dimensions.x, dimensions.y, 0.0f) * 
		DirectX::XMMatrixRotationZ(angle) *
		DirectX::XMMatrixTranslation(finalPos.x, finalPos.y, 0.0f);
	
	pGfx->RenderModel2D(viewMatrix, worldMatrix, pGfx->GetOrthoMatrix(), cPTexture->GetTexture(), pVertexBuffer,
		pIndexBuffer, cPQuad->GetIndexCount());
}

void Image::Draw(const DirectX::XMFLOAT2& pos, float angle, float scaleFactor,
	const DirectX::XMMATRIX& viewMatrix, GraphicsDxD* pGfx) const
{
	ID3D11Buffer* pVertexBuffer = cPQuad->GetVertexBuffer();
	ID3D11Buffer* pIndexBuffer = cPQuad->GetIndexBuffer();

	DirectX::XMFLOAT2 finalPos = { pos.x,
		-pos.y };

	DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixScaling(cWidth * scaleFactor, cHeight * scaleFactor, 0.0f) *
		DirectX::XMMatrixRotationZ(angle)*
		DirectX::XMMatrixTranslation(finalPos.x, finalPos.y, 0.0f);

	pGfx->RenderModel2D(viewMatrix, worldMatrix, pGfx->GetOrthoMatrix(), cPTexture->GetTexture(), pVertexBuffer,
		pIndexBuffer, cPQuad->GetIndexCount());
}