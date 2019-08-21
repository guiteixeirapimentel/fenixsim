#pragma once
#include <d3d11.h>
#include <DirectXMath.h>

#include <fstream>

#include "TextureShader.h"
#include "TextureShader2D.h"
#include "Shader3D.h"
#include "ShaderLights3D.h"
#include "Texture.h"

class Quad;
class Cube;


class GraphicsDxD
{
public:
	GraphicsDxD(HWND hWnd);
	GraphicsDxD() = delete;
	GraphicsDxD(const GraphicsDxD& gfx) = delete;
	~GraphicsDxD();

	void IniciarCena();
	void TerminarCena();

	void RenderModel2D(const DirectX::XMMATRIX& viewM, const DirectX::XMMATRIX& worldM, const DirectX::XMMATRIX& projM,
		ID3D11ShaderResourceView* pTexture, ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer, 
		unsigned int indexCount);

	void RenderModel3D(const DirectX::XMMATRIX& viewM, const DirectX::XMMATRIX& worldM, const DirectX::XMMATRIX& projM,
		ID3D11ShaderResourceView* pTexture, ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer,
		unsigned int indexCount);

	void RenderModel3DLights(const DirectX::XMMATRIX& viewM, const DirectX::XMMATRIX& worldM, const DirectX::XMMATRIX& projM,
		ID3D11ShaderResourceView* pTexture, const Material& mat, ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer,
		unsigned int indexCount);

	void SetBuffers2D(ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer);

	void SetBuffers3D(ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer);
	
	void SetBuffers3DLights(ID3D11Buffer* pVertexBuffer, ID3D11Buffer* pIndexBuffer);

	void SetLightAndEyePos(const PointLight& pointLight, const DirectX::XMFLOAT4& eyePos);

	bool SetShaderParameters2D(const DirectX::XMMATRIX& viewM, const DirectX::XMMATRIX& worldM,
		const DirectX::XMMATRIX& projM, ID3D11ShaderResourceView* pTexture);

	bool SetShaderParameters3D(const DirectX::XMMATRIX& viewM, const DirectX::XMMATRIX& worldM,
		const DirectX::XMMATRIX& projM, ID3D11ShaderResourceView* pTexture);

	bool SetShaderParameters3DLights(const DirectX::XMMATRIX& viewM, const DirectX::XMMATRIX& worldM,
		const DirectX::XMMATRIX& projM, ID3D11ShaderResourceView* pTexture, const Material& material);

	bool OnWindowResize();

	void Enable3D();
	void Enable3DAndLights();

	void Enable2D();

	void RenderShader2D(int indexCount);

	void RenderShader3D(int indexCount);

	void RenderShader3DWithLights(int indexCount);

	ID3D11Device* GetPDevice() const
	{
		return cPDevice;
	}

	ID3D11DeviceContext* GetPDeviceContext() const
	{
		return cPDeviceContext;
	}

	const Quad* GetPQuad() const;
	const Cube* GetPCube() const;
	
	DirectX::XMMATRIX GetProjMatrix() const
	{
		return cProjectionMatrix;
	}
	DirectX::XMMATRIX GetOrthoMatrix() const
	{
		return cOrthoMatrix;
	}
	UINT32 GetWndWidth() const
	{
		return cWndWidth;
	}
	UINT32 GetWndHeight() const
	{
		return cWndHeight;
	}
private:
	HWND cHWnd;
	UINT32 cWndWidth;
	UINT32 cWndHeight;
	bool cFullScreen;
	bool cVSync;

	bool c3DEnabled;
	bool cLightsEnabled;

	UINT cMaxMultSampling;

	IDXGISwapChain* cPSwapChain;
	ID3D11Device* cPDevice;
	ID3D11DeviceContext* cPDeviceContext;
	ID3D11RenderTargetView* cPRenderTargetView;
	ID3D11Texture2D* cPDepthStencilBuffer;
	ID3D11DepthStencilState* cPDepthStencilState;
	ID3D11DepthStencilState* cPDepthDisabledStencilState;
	ID3D11DepthStencilView* cPDepthStencilView;
	ID3D11RasterizerState* cPRasterState;
	ID3D11BlendState* cPBlendState;
#ifdef _DEBUG
	ID3D11Debug *cPDebug;
#endif

	TextureShader2D* cPTextureShader2D;
	Shader3D* cPShader3D;
	ShaderLights3D* cPShaderLights3D;
	
	DirectX::XMMATRIX  cOrthoMatrix;
	DirectX::XMMATRIX  cProjectionMatrix;

	Quad* cPQuad;
	Cube* cPCube;
};