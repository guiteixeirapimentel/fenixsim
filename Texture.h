#pragma once
#include <fstream>
#include <string>

#include <d3d11.h>


class Texture
{
public:
	Texture(const std::wstring& fileName, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext);
	Texture(const char* buffer, UINT width, UINT height, ID3D11Device* pDevice, 
		ID3D11DeviceContext* pDeviceContext);
	~Texture();

	ID3D11ShaderResourceView* GetTexture() const;

	unsigned int GetWidth() const;
	unsigned int GetHeight() const;

private:
	ID3D11Texture2D* cPTexture;
	ID3D11ShaderResourceView* cPTextureView;

	unsigned int cWidth;
	unsigned int cHeight;
};