#pragma once
#include "Shader.h"
#include <DirectXMath.h>

struct VertexTypeTextureShader2D
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT2 texture;
};

struct MatrixBufferTypeTextureShader2D
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX orthoMatrix;
};

class TextureShader2D : public Shader
{
public:
	TextureShader2D(HWND hWnd, GraphicsDxD& gfx);
	~TextureShader2D();

};