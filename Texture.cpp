#include "Texture.h"
#include <memory>
#include <assert.h>

#include "GDIPlusManager.h"
#include <gdiplus.h>

Texture::Texture(const std::wstring& fileName, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:
	cPTexture(nullptr),
	cPTextureView(nullptr)
{
	GDIPlusManager gdiMan;
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int pitch = 0;
	char* pBuffer = nullptr;

	Gdiplus::Bitmap bitmap(fileName.c_str(), TRUE);
	if (bitmap.GetLastStatus() != Gdiplus::Status::Ok)
	{
		assert(1);
	}

	width = bitmap.GetWidth();
	pitch = (width * 4) * sizeof(unsigned char);
	height = bitmap.GetHeight();
	pBuffer = new char[height * width * 4];

	cWidth = width;
	cHeight = height;

	for (unsigned int y = 0; y < height; y++)
	{
		for (unsigned int x = 0; x < width; x++)
		{
			Gdiplus::Color c;
			bitmap.GetPixel(x, y, &c);
			reinterpret_cast<uint32_t*>(pBuffer)[(y * width) + x] = c.GetValue();

		}
	}

	HRESULT hResult;

	D3D11_TEXTURE2D_DESC texDesc;

	texDesc.Height = height;
	texDesc.Width = width;
	texDesc.MipLevels = 0;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;//DXGI_FORMAT_B8G8R8A8_UNORM;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

	hResult = pDevice->CreateTexture2D(&texDesc, NULL, &cPTexture);


	pDeviceContext->UpdateSubresource(cPTexture, 0, NULL, pBuffer, pitch, 0);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = -1;

	hResult = pDevice->CreateShaderResourceView(cPTexture, &srvDesc, &cPTextureView);

	pDeviceContext->GenerateMips(cPTextureView);
	
	delete[] pBuffer;
	pBuffer = nullptr;
}

Texture::Texture(const char* pBuffer, UINT width, UINT height, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
	:
	cPTexture(nullptr),
	cPTextureView(nullptr)
	{

	if (!pBuffer || height == 0 || width == 0)
	{
		MessageBox(NULL, L"Invalid Buffer nullptr! Texture Constructor.", L"Error", 0);
	}

		HRESULT hResult;

		D3D11_TEXTURE2D_DESC texDesc;

		unsigned int pitch = (width * 4) * sizeof(unsigned char);

		texDesc.Height = height;
		texDesc.Width = width;
		texDesc.MipLevels = 1;
		texDesc.ArraySize = 1;
		texDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;//DXGI_FORMAT_B8G8R8A8_UNORM;
		texDesc.SampleDesc.Count = 1;
		texDesc.SampleDesc.Quality = 0;
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
		texDesc.CPUAccessFlags = 0;
		texDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;

		hResult = pDevice->CreateTexture2D(&texDesc, NULL, &cPTexture);


		pDeviceContext->UpdateSubresource(cPTexture, 0, NULL, pBuffer, pitch, 0);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

		srvDesc.Format = texDesc.Format;
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MostDetailedMip = 0;
		srvDesc.Texture2D.MipLevels = -1;

		hResult = pDevice->CreateShaderResourceView(cPTexture, &srvDesc, &cPTextureView);

		pDeviceContext->GenerateMips(cPTextureView);
}

Texture::~Texture()
{
	if (cPTextureView)
	{
		cPTextureView->Release();
		cPTextureView = 0;
	}

	// Release the texture.
	if (cPTexture)
	{
		cPTexture->Release();
		cPTexture = 0;
	}
}

ID3D11ShaderResourceView* Texture::GetTexture() const
{
	return cPTextureView;
}

unsigned int Texture::GetHeight() const
{
	return cHeight;
}

unsigned int Texture::GetWidth() const
{
	return cWidth;
}