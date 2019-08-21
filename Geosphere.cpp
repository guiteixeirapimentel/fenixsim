#include "Geosphere.h"
#include "Shader3D.h"

float AngleFromXY(float x, float y)
{
	constexpr float PPI = 3.1415926535f;
	float theta = 0.0f;

	// Quadrant I or IV
	if (x >= 0.0f)
	{
		// If x = 0, then atanf(y/x) = +pi/2 if y > 0
		//                atanf(y/x) = -pi/2 if y < 0
		theta = atanf(y / x); // in [-pi/2, +pi/2]

		if (theta < 0.0f)
			theta += 2.0f*PPI; // in [0, 2*pi).
	}

	// Quadrant II or III
	else
		theta = atanf(y / x) + PPI; // in [0, 2*pi).

	return theta;
}

Geosphere::Geosphere(float radius, UINT numSubdivisions, ID3D11Device* pDevice)
	:
	cPIndexBuffer(nullptr),
	cPVertexBuffer(nullptr)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	CreateGeosphere(radius, numSubdivisions);

	// Set the number of vertices in the vertex array.
	cVertexCount = (unsigned int)cVertices.size();

	// Set the number of indices in the index array.
	cIndexCount = (unsigned int)cIndices.size();

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexTypeShader3D) * cVertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
	vertexData.pSysMem = cVertices.data();
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
	indexData.pSysMem = cIndices.data();
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = pDevice->CreateBuffer(&indexBufferDesc, &indexData, &cPIndexBuffer);

	if (FAILED(result))
	{
		assert(1);
	}

	cVertices.clear();
	cIndices.clear();
}

Geosphere::~Geosphere()
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

ID3D11Buffer* Geosphere::GetVertexBuffer() const
{
	return cPVertexBuffer;
}

ID3D11Buffer* Geosphere::GetIndexBuffer() const
{
	return cPIndexBuffer;
}

unsigned int Geosphere::GetIndexCount() const
{
	return cIndexCount;
}
void Geosphere::CreateGeosphere(float radius, UINT numSubdivisions)
{
	// Put a cap on the number of subdivisions.
	numSubdivisions = min(numSubdivisions, 5u);

	// Approximate a sphere by tessellating an icosahedron.

	const float X = 0.525731f;
	const float Z = 0.850651f;

	DirectX::XMFLOAT4 pos[12] =
	{
		DirectX::XMFLOAT4(-X, 0.0f, Z, 0.0f),  DirectX::XMFLOAT4(X, 0.0f, Z, 0.0f),
		DirectX::XMFLOAT4(-X, 0.0f, -Z, 0.0f), DirectX::XMFLOAT4(X, 0.0f, -Z, 0.0f),
		DirectX::XMFLOAT4(0.0f, Z, X, 0.0f),   DirectX::XMFLOAT4(0.0f, Z, -X, 0.0f),
		DirectX::XMFLOAT4(0.0f, -Z, X, 0.0f),  DirectX::XMFLOAT4(0.0f, -Z, -X, 0.0f),
		DirectX::XMFLOAT4(Z, X, 0.0f, 0.0f),   DirectX::XMFLOAT4(-Z, X, 0.0f, 0.0f),
		DirectX::XMFLOAT4(Z, -X, 0.0f, 0.0f),  DirectX::XMFLOAT4(-Z, -X, 0.0f, 0.0f)
	};

	DWORD k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	cVertices.resize(12);
	cIndices.resize(60);

	for (UINT i = 0; i < 12; ++i)
		cVertices[i].position = pos[i];

	for (UINT i = 0; i < 60; ++i)
		cIndices[i] = k[i];

	for (UINT i = 0; i < numSubdivisions; ++i)
		Subdivide();

	// Project vertices onto sphere and scale.
	for (UINT i = 0; i < cVertices.size(); ++i)
	{
		// Project onto unit sphere.
		DirectX::XMVECTOR n = DirectX::XMVector3Normalize(XMLoadFloat4(&cVertices[i].position));

		// Project onto sphere.
		DirectX::XMVECTOR p = DirectX::XMVectorScale(n, radius);

		DirectX::XMStoreFloat4(&cVertices[i].position, p);

		DirectX::XMStoreFloat3(&cVertices[i].normal, n);

		// Derive texture coordinates from spherical coordinates.
		const float theta = AngleFromXY(cVertices[i].position.x, cVertices[i].position.z);

		const float phi = acosf(cVertices[i].position.y / radius);

		cVertices[i].texture.x = theta / DirectX::XM_2PI;
		cVertices[i].texture.y = phi / DirectX::XM_PI;		
	}
}

void Geosphere::Subdivide()
{
	// Save a copy of the input geometry.
	std::vector<VertexTypeShader3D> copyVertices = std::move(cVertices);
	std::vector<DWORD> copyIndices = std::move(cIndices);


	cVertices.resize(0);
	cIndices.resize(0);

	//       v1
	//       *
	//      / \
		//     /   \
	//  m0*-----*m1
//   / \   / \
	//  /   \ /   \
	// *-----*-----*
// v0    m2     v2

	UINT numTris = (UINT) (copyIndices.size() / 3);
	for (UINT i = 0; i < numTris; ++i)
	{
		VertexTypeShader3D v0 = copyVertices[copyIndices[i * 3 + 0]];
		VertexTypeShader3D v1 = copyVertices[copyIndices[i * 3 + 1]];
		VertexTypeShader3D v2 = copyVertices[copyIndices[i * 3 + 2]];

		//
		// Generate the midpoints.
		//

		VertexTypeShader3D m0, m1, m2;

		// For subdivision, we just care about the position component.  We derive the other
		// vertex components in CreateGeosphere.

		m0.position = DirectX::XMFLOAT4(
			0.5f*(v0.position.x + v1.position.x),
			0.5f*(v0.position.y + v1.position.y),
			0.5f*(v0.position.z + v1.position.z), 1.0f);

		m1.position = DirectX::XMFLOAT4(
			0.5f*(v1.position.x + v2.position.x),
			0.5f*(v1.position.y + v2.position.y),
			0.5f*(v1.position.z + v2.position.z), 1.0f);

		m2.position = DirectX::XMFLOAT4(
			0.5f*(v0.position.x + v2.position.x),
			0.5f*(v0.position.y + v2.position.y),
			0.5f*(v0.position.z + v2.position.z), 1.0f);

		//
		// Add new geometry.
		//

		cVertices.push_back(v0); // 0
		cVertices.push_back(v1); // 1
		cVertices.push_back(v2); // 2
		cVertices.push_back(m0); // 3
		cVertices.push_back(m1); // 4
		cVertices.push_back(m2); // 5

		cIndices.push_back(i * 6 + 0);
		cIndices.push_back(i * 6 + 3);
		cIndices.push_back(i * 6 + 5);

		cIndices.push_back(i * 6 + 3);
		cIndices.push_back(i * 6 + 4);
		cIndices.push_back(i * 6 + 5);
		
		cIndices.push_back(i * 6 + 5);
		cIndices.push_back(i * 6 + 4);
		cIndices.push_back(i * 6 + 2);
		
		cIndices.push_back(i * 6 + 3);
		cIndices.push_back(i * 6 + 1);
		cIndices.push_back(i * 6 + 4);
	}
}