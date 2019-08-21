#include "FinModel.h"
#include "TextureShader2D.h"

FinModel::FinModel(float chordRoot, float chordTrailling, float m, float s, float thickness, ID3D11Device* pDevice)
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
	
	constexpr float one = 1.0f;
	constexpr float zero = 0.0f;
	constexpr float half = 1.0f / 2.0f;
	constexpr float onethird = 1.0f / 3.0f;
	constexpr float twothirds = 2.0f / 3.0f;

	constexpr float onefourth = 1.0f / 4;
	constexpr float twofourths = 1.0f / 4.0f;
	constexpr float threefourths = 1 / 3.0f;
	

	const VertexTypeShader3D vertices[] =
	{
		// Front Face
		{ { 0.0f, -chordRoot / 2.0f, -thickness / 2.0f, 0.0f },{ onethird, half },{ 0.0f, 0.0f, -1.0f } } ,
		{ { 0.0f,  chordRoot / 2.0f, -thickness / 2.0f, 0.0f },{ onethird, zero },{ 0.0f, 0.0f, -1.0f } },
		{ { s,  (chordTrailling / 2.0f) - (m / 2.0f), -thickness / 2.0f, 0.0f },{ twothirds, zero },{ 0.0f, 0.0f, -1.0f } },
		{ { s, (-chordTrailling / 2.0f) - (m / 2.0f), -thickness / 2.0f, 0.0f },{ twothirds, half },{ 0.0f, 0.0f, -1.0f } },

		// Back Face
		{ { 0.0f, -chordRoot / 2.0f, thickness / 2.0f, 0.0f },{ twothirds,one },{ 0.0f, 0.0f, 1.0f } },
		{ { s, (-chordTrailling / 2.0f) - (m / 2.0f), thickness / 2.0f, 0.0f },{ onethird , one },{ 0.0f, 0.0f, 1.0f } },
		{ { s,  (chordTrailling / 2.0f) - (m / 2.0f), thickness / 2.0f, 0.0f },{ onethird,  half },{ 0.0f, 0.0f, 1.0f } },
		{ { 0.0f,  chordRoot / 2.0f, thickness / 2.0f, 0.0f },{ twothirds, half },{ 0.0f, 0.0f, 1.0f } },

		// Top Face
		{ { 0.0f, chordRoot / 2.0f, -thickness / 2.0f, 0.0f },{ zero, one },{ 0.0f, 1.0f, 0.0f } },
		{ { 0.0f, chordRoot / 2.0f,  thickness / 2.0f,0.0f },{ zero, half },{ 0.0f, 1.0f, 0.0f } },
		{ { s, (chordTrailling / 2.0f) - (m / 2.0f),  thickness / 2.0f, 0.0f },{ onethird, half },{ 0.0f, 1.0f, 0.0f } },
		{ { s, (chordTrailling / 2.0f) - (m / 2.0f), -thickness / 2.0f, 0.0f },{ onethird, one },{ 0.0f, 1.0f, 0.0f } },

		// Bottom Face
		{ { 0.0f, -chordRoot / 2.0f, -thickness / 2.0f,0.0f },{ one, half },{ 0.0f, -1.0f, 0.0f } },
		{ { s, (-chordTrailling / 2.0f) - (m / 2.0f), -thickness / 2.0f,0.0f },{ twothirds, half },{ 0.0f, -1.0f, 0.0f } },
		{ { s, (-chordTrailling / 2.0f) - (m / 2.0f),  thickness / 2.0f,0.0f },{ twothirds, zero },{ 0.0f, -1.0f, 0.0f } },
		{ { 0.0f, -chordRoot / 2.0f,  thickness / 2.0f,0.0f },{ one, zero },{ 0.0f, -1.0f, 0.0f } },

		// Left Face
		{ { 0.0f, -chordRoot / 2.0f,  thickness / 2.0f,0.0f },{ zero, half },{ -1.0f, 0.0f, 0.0f } },
		{ { 0.0f,  chordRoot / 2.0f,  thickness / 2.0f,0.0f },{ zero, zero },{ -1.0f, 0.0f, 0.0f } },
		{ { 0.0f,  chordRoot / 2.0f, -thickness / 2.0f,0.0f },{ onethird, zero },{ -1.0f, 0.0f, 0.0f } },
		{ { 0.0f, -chordRoot / 2.0f, -thickness / 2.0f,0.0f },{ onethird, half },{ -1.0f, 0.0f, 0.0f } },

		// Right Face
		{ { s, -(chordTrailling / 2.0f) - (m / 2.0f), -thickness / 2.0f,0.0f },{ twothirds, one },{ 1.0f, 0.0f, 0.0f } },
		{ { s,  (chordTrailling / 2.0f) - (m / 2.0f), -thickness / 2.0f,0.0f },{ twothirds, half },{ 1.0f, 0.0f, 0.0f } },
		{ { s,  (chordTrailling / 2.0f) - (m / 2.0f),  thickness / 2.0f,0.0f },{ one, half },{ 1.0f, 0.0f, 0.0f } },
		{ { s, -(chordTrailling / 2.0f) - (m / 2.0f),  thickness / 2.0f,0.0f },{ one, one },{ 1.0f, 0.0f, 0.0f } }
	};

	// Load the index array with data.

	
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

FinModel::~FinModel()
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

ID3D11Buffer* FinModel::GetVertexBuffer() const
{
	return cPVertexBuffer;
}

ID3D11Buffer* FinModel::GetIndexBuffer() const
{
	return cPIndexBuffer;
}

unsigned int FinModel::GetIndexCount() const
{
	return cIndexCount;
}