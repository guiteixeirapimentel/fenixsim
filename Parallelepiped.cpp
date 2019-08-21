#include "Parallelepiped.h"
#include "TextureShader2D.h"

Parallelepiped::Parallelepiped(float widthBottom, float widthTop, float heightFront, float heightBack,
	float depthBottom, float depthTop, ID3D11Device* pDevice)
	:
	cPIndexBuffer(nullptr),
	cPVertexBuffer(nullptr)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	// Set the number of vertices in the vertex array.
	cVertexCount = 24;

	// Set the number of indices in the index array.
	cIndexCount = 36;

	// Create the vertex array.
	//vertices = new VertexTypeTextureShader3D[cVertexCount];

	// Load the vertex array with data.

	/*vertices[0].position = DirectX::XMFLOAT4(-0.5f, -0.5f, 0.5f, 0.0f);
	vertices[0].texture = DirectX::XMFLOAT2(0.0f, 1.0f);

	vertices[1].position = DirectX::XMFLOAT4(-0.5f, 0.5f, 0.5f, 0.0f);
	vertices[1].texture = DirectX::XMFLOAT2(0.0f, 0.0f);

	vertices[2].position = DirectX::XMFLOAT4(0.5f, -0.5f, 0.5f, 0.0f);
	vertices[2].texture = DirectX::XMFLOAT2(1.0f, 1.0f);

	vertices[3].position = DirectX::XMFLOAT4(0.5f, 0.5f, 0.5f, 0.0f);
	vertices[3].texture = DirectX::XMFLOAT2(1.0f, 0.0f);

	vertices[4].position = DirectX::XMFLOAT4(0.5f, 0.5f, -0.5f, 0.0f);
	vertices[4].texture = DirectX::XMFLOAT2(1.0f, 0.0f);

	vertices[5].position = DirectX::XMFLOAT4(-0.5f, 0.5f, -0.5f, 0.0f);
	vertices[5].texture = DirectX::XMFLOAT2(0.0f, 1.0f);

	vertices[6].position = DirectX::XMFLOAT4(-0.5f, -0.5f, -0.5f, 0.0f);
	vertices[6].texture = DirectX::XMFLOAT2(0.0f, 0.0f);

	vertices[7].position = DirectX::XMFLOAT4(0.5f, -0.5f, -0.5f, 0.0f);
	vertices[7].texture = DirectX::XMFLOAT2(0.0f, 0.0f);
	*/

	constexpr float one = 1.0f;
	constexpr float zero = 0.0f;
	constexpr float half = 1.0f / 2.0f;
	constexpr float onethird = 1.0f / 3.0f;
	constexpr float twothirds = 2.0f / 3.0f;

	constexpr float onefourth = 1.0f / 4;
	constexpr float twofourths = 1.0f / 4.0f;
	constexpr float threefourths = 1 / 3.0f;

	VertexTypeShader3D vertices[] =
	{
		// Front Face
		{ { -widthBottom/2.0f, -heightFront / 2.0f, -depthBottom/2.0f, 0.0f },{ onethird, half },{ 0.0f, 0.0f, -1.0f } } ,
		{ { -widthTop/2.0f,  heightFront / 2.0f, -depthTop / 2.0f, 0.0f },{ onethird, zero },{ 0.0f, 0.0f, -1.0f } },
		{ { widthTop / 2.0f,  heightFront / 2.0f, -depthTop / 2.0f, 0.0f },{ twothirds, zero },{ 0.0f, 0.0f, -1.0f } },
		{ { widthBottom / 2.0f, -heightFront / 2.0f, -depthBottom / 2.0f, 0.0f },{ twothirds, half },{ 0.0f, 0.0f, -1.0f } },

		// Back Face
		{ { -widthBottom / 2.0f, -heightBack / 2.0f, depthBottom / 2.0f, 0.0f },{ twothirds,one },{ 0.0f, 0.0f, 1.0f } },
		{ { widthBottom / 2.0f, -heightBack / 2.0f, depthBottom / 2.0f, 0.0f },{ onethird , one },{ 0.0f, 0.0f, 1.0f } },
		{ { widthTop / 2.0f,  heightBack / 2.0f, depthTop / 2.0f, 0.0f },{ onethird,  half },{ 0.0f, 0.0f, 1.0f } },
		{ { -widthTop / 2.0f,  heightBack / 2.0f, depthTop / 2.0f, 0.0f },{ twothirds, half },{ 0.0f, 0.0f, 1.0f } },

		// Top Face
		{ { -widthTop / 2.0f, heightFront / 2.0f, -depthTop / 2.0f, 0.0f },{ zero, one },{ 0.0f, 1.0f, 0.0f } },
		{ { -widthTop / 2.0f, heightBack / 2.0f,  depthTop / 2.0f,0.0f },{ zero, half },{ 0.0f, 1.0f, 0.0f } },
		{ { widthTop / 2.0f, heightBack / 2.0f,  depthTop / 2.0f, 0.0f },{ onethird, half },{ 0.0f, 1.0f, 0.0f } },
		{ { widthTop / 2.0f, heightFront / 2.0f, -depthTop / 2.0f, 0.0f },{ onethird, one },{ 0.0f, 1.0f, 0.0f } },

		// Bottom Face
		{ { -widthBottom / 2.0f, -heightFront / 2.0f, -depthBottom / 2.0f,0.0f },{ one, half },{ 0.0f, -1.0f, 0.0f } },
		{ { widthBottom / 2.0f, -heightFront / 2.0f, -depthBottom / 2.0f,0.0f },{ twothirds, half },{ 0.0f, -1.0f, 0.0f } },
		{ { widthBottom / 2.0f, -heightBack / 2.0f,  depthBottom / 2.0f,0.0f },{ twothirds, zero },{ 0.0f, -1.0f, 0.0f } },
		{ { -widthBottom / 2.0f, -heightBack / 2.0f,  depthBottom / 2.0f,0.0f },{ one, zero },{ 0.0f, -1.0f, 0.0f } },

		// Left Face
		{ { -widthBottom / 2.0f, -heightBack / 2.0f,  depthBottom / 2.0f,0.0f },{ zero, half },{ -1.0f, 0.0f, 0.0f } },
		{ { -widthTop / 2.0f,  heightBack / 2.0f,  depthTop / 2.0f,0.0f },{ zero, zero },{ -1.0f, 0.0f, 0.0f } },
		{ { -widthTop / 2.0f,  heightFront / 2.0f, -depthTop / 2.0f,0.0f },{ onethird, zero },{ -1.0f, 0.0f, 0.0f } },
		{ { -widthBottom / 2.0f, -heightFront / 2.0f, -depthBottom / 2.0f,0.0f },{ onethird, half },{ -1.0f, 0.0f, 0.0f } },

		// Right Face
		{ { widthBottom / 2.0f, -heightFront / 2.0f, -depthBottom / 2.0f,0.0f },{ twothirds, one },{ 1.0f, 0.0f, 0.0f } },
		{ { widthTop / 2.0f,  heightFront / 2.0f, -depthTop / 2.0f,0.0f },{ twothirds, half },{ 1.0f, 0.0f, 0.0f } },
		{ { widthTop / 2.0f,  heightBack / 2.0f,  depthTop / 2.0f,0.0f },{ one, half },{ 1.0f, 0.0f, 0.0f } },
		{ { widthBottom / 2.0f, -heightBack / 2.0f,  depthBottom / 2.0f,0.0f },{ one, one },{ 1.0f, 0.0f, 0.0f } }
	};

	// Load the index array with data.

	//FACE 1 frente
	/*indices[0] = 2;
	indices[1] = 1;
	indices[2] = 0;

	indices[3] = 3;
	indices[4] = 1;
	indices[5] = 2;

	//FACE 2 cima
	indices[6] = 3;
	indices[7] = 5;
	indices[8] = 1;

	indices[9] = 4;
	indices[10] = 5;
	indices[11] = 3;

	//FACE 3 lado direito
	indices[12] = 3;
	indices[13] = 7;
	indices[14] = 4;

	indices[15] = 2;
	indices[16] = 7;
	indices[17] = 3;

	//FACE 4 baixo
	indices[18] = 6;
	indices[19] = 7;
	indices[20] = 2;

	indices[21] = 0;
	indices[22] = 6;
	indices[23] = 2;

	//FACE 5 lado esquerdo
	indices[24] = 6;
	indices[25] = 0;
	indices[26] = 1;

	indices[27] = 5;
	indices[28] = 6;
	indices[29] = 1;

	//FACE 6 trás
	indices[30] = 4;
	indices[31] = 6;
	indices[32] = 5;

	indices[33] = 7;
	indices[34] = 6;
	indices[35] = 4;
	*/

	DWORD indices[] = {
		// Front Face
		0,  1,  2,
		0,  2,  3,

		// Back Face
		4,  5,  6,
		4,  6,  7,

		// Top Face
		8,  9, 10,
		8, 10, 11,

		// Bottom Face
		12, 13, 14,
		12, 14, 15,

		// Left Face
		16, 17, 18,
		16, 18, 19,

		// Right Face
		20, 21, 22,
		20, 22, 23
	};

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexTypeShader3D) * cVertexCount;
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
}

Parallelepiped::~Parallelepiped()
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

ID3D11Buffer* Parallelepiped::GetVertexBuffer() const
{
	return cPVertexBuffer;
}

ID3D11Buffer* Parallelepiped::GetIndexBuffer() const
{
	return cPIndexBuffer;
}

unsigned int Parallelepiped::GetIndexCount() const
{
	return cIndexCount;
}