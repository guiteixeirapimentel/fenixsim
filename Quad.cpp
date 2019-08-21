#include "Quad.h"
#include "TextureShader2D.h"

Quad::Quad(ID3D11Device* pDevice)
	:
	cPIndexBuffer(nullptr),
	cPVertexBuffer(nullptr)
{
	VertexTypeTextureShader2D* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array.
	cVertexCount = 4;

	// Set the number of indices in the index array.
	cIndexCount = 6;

	// Create the vertex array.
	vertices = new VertexTypeTextureShader2D[cVertexCount];
	if (!vertices)
	{
		assert(1);
	}

	// Create the index array.
	indices = new unsigned long[cIndexCount];
	if (!indices)
	{
		assert(1);
	}
	
	// Load the vertex array with data.
	vertices[0].position = DirectX::XMFLOAT4(-0.5f, 0.5f, 0.0f,0.0f);
	vertices[0].texture = DirectX::XMFLOAT2(0.0f, 0.0f);

	vertices[1].position = DirectX::XMFLOAT4(0.5f, 0.5f, 0.0f,0.0f);
	vertices[1].texture = DirectX::XMFLOAT2(1.0f, 0.0f);

	vertices[2].position = DirectX::XMFLOAT4(0.5f, -0.5f, 0.0f,0.0f);
	vertices[2].texture = DirectX::XMFLOAT2(1.0f, 1.0f);

	vertices[3].position = DirectX::XMFLOAT4(-0.5f, -0.5f, 0.0f,0.0f);
	vertices[3].texture = DirectX::XMFLOAT2(0.0f, 1.0f);

	// Load the index array with data.
	indices[0] = 0;  
	indices[1] = 1;  
	indices[2] = 2;  

	indices[3] = 0;  
	indices[4] = 2;  
	indices[5] = 3;  
					 // Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexTypeTextureShader2D) * cVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = pDevice->CreateBuffer(&vertexBufferDesc, &vertexData, &cPVertexBuffer);
	if (FAILED(result))
	{
		assert(1);
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * cIndexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &cPIndexBuffer);
	if (FAILED(result))
	{
		assert(1);
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = nullptr;

	delete[] indices;
	indices = nullptr;
}

Quad::~Quad()
{
	if (cPIndexBuffer)
	{
		cPIndexBuffer->Release();
		cPIndexBuffer = nullptr;
	}

	if (cPVertexBuffer)
	{
		cPVertexBuffer->Release();
		cPVertexBuffer = nullptr;
	}
}

ID3D11Buffer* Quad::GetVertexBuffer() const
{
	return cPVertexBuffer;
}

ID3D11Buffer* Quad::GetIndexBuffer() const
{
	return cPIndexBuffer;
}

unsigned int Quad::GetIndexCount() const
{
	return cIndexCount;
}