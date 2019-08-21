#pragma once
#include "Shader.h"
#include <DirectXMath.h>

struct VertexTypeLightsShader3D
{
	DirectX::XMFLOAT4 position;
	DirectX::XMFLOAT2 texture;
	DirectX::XMFLOAT3 normal;
};

struct Material
{
	DirectX::XMFLOAT4 cAmbient;
	DirectX::XMFLOAT4 cDiffuse;
	DirectX::XMFLOAT4 cSpecular; // w = SpecPower
	DirectX::XMFLOAT4 cReflect;
};

struct PointLight
{
	DirectX::XMFLOAT4 cAmbient;
	DirectX::XMFLOAT4 cDiffuse;
	DirectX::XMFLOAT4 cSpecular;
	
	DirectX::XMFLOAT3 cPosition;
	float range;

	DirectX::XMFLOAT4 cAtt;
};

struct MatrixBufferTypeLightsShader3D
{
	DirectX::XMMATRIX worldMatrix;
	DirectX::XMMATRIX viewMatrix;
	DirectX::XMMATRIX orthoMatrix;
	Material cMaterial;
};

struct BufferPerFrame
{
	PointLight bPointLight;
	DirectX::XMFLOAT4 bEyePosW;
};

class ShaderLights3D : public Shader
{
public:
	ShaderLights3D(HWND hWnd, GraphicsDxD& gfx);
	~ShaderLights3D();
	
	ID3D11Buffer* GetPPerFrameBuffer()
	{
		return cPPerFrameBuffer;
	}
private:
	ID3D11Buffer* cPPerFrameBuffer;

};