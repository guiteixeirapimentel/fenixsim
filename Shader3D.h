#pragma once
#include "Shader.h"
#include <DirectXMath.h>

struct VertexTypeShader3D
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT2 texture;
	DirectX::XMFLOAT3 normal;
};

struct MatrixBufferTypeShader3D
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX orthoMatrix;
};

class Shader3D : public Shader
{
public:
	Shader3D(HWND hWnd, GraphicsDxD& gfx);
	~Shader3D();

};