#pragma once
#include <d3d11.h>
#include <fstream>

#include <d3dcompiler.h>
#pragma comment(lib, "D3DCompiler.lib")

class GraphicsDxD;

class Shader
{
public:
	Shader(HWND hWnd, ID3D11Device* pDevice);
	Shader(const Shader&) = delete;

	~Shader();

	ID3D11Buffer* GetPMatrixBuffer()
	{
		return cPMatrixBuffer;
	}
	ID3D11InputLayout* GetPLayout()
	{
		return cPLayout;
	}

	ID3D11VertexShader* GetPVertexShader()
	{
		return cPVertexShader;
	}
	ID3D11PixelShader* GetPPixelShader()
	{
		return cPPixelShader;
	}
	ID3D11SamplerState* GetPSamplerState()
	{
		return cPSampleState;
	}

protected:
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

protected:
	ID3D11VertexShader* cPVertexShader;
	ID3D11PixelShader* cPPixelShader;

	ID3D11InputLayout* cPLayout;
	ID3D11Buffer* cPMatrixBuffer;

	ID3D11SamplerState* cPSampleState;

	UINT i = 0;
};