#pragma once
#include "Shader.h"
#include <DirectXMath.h>

struct VertexType
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 texture;
};

struct MatrixBufferType
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX orthoMatrix;
};

class TextureShader : public Shader
{
public:
	TextureShader(HWND hWnd, GraphicsDxD& gfx);
	~TextureShader();
	
};