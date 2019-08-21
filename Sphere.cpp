#include "Sphere.h"
#include "Shader3D.h"

Sphere::Sphere(float radius, UINT sliceCount, UINT stackCount, ID3D11Device* pDevice)
	:
	cPIndexBuffer(nullptr),
	cPVertexBuffer(nullptr)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	CreateSphere(radius, sliceCount, stackCount);


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

Sphere::~Sphere()
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

ID3D11Buffer* Sphere::GetVertexBuffer() const
{
	return cPVertexBuffer;
}

ID3D11Buffer* Sphere::GetIndexBuffer() const
{
	return cPIndexBuffer;
}

unsigned int Sphere::GetIndexCount() const
{
	return cIndexCount;
}

void Sphere::CreateSphere(float radius, UINT sliceCount, UINT stackCount)
{
	cVertices.clear();
	cIndices.clear();

	//
	// Compute the vertices stating at the top pole and moving down the stacks.
	//

	// Poles: note that there will be texture coordinate distortion as there is
	// not a unique point on the texture map to assign to the pole when mapping
	// a rectangular texture onto a sphere.
	VertexTypeShader3D topVertex{ {0.0f, +radius, 0.0f, 1.0f}, { 0.0f, 0.0f }, {0.0f, +1.0f, 0.0f} };
	VertexTypeShader3D bottomVertex{ {0.0f, -radius, 0.0f, 1.0f},{ 0.0f, 1.0f }, { 0.0f, -1.0f, 0.0f} };

	cVertices.push_back(topVertex);

	float phiStep = DirectX::XM_PI / stackCount;
	float thetaStep = 2.0f*DirectX::XM_PI / sliceCount;

	// Compute vertices for each stack ring (do not count the poles as rings).
	for (UINT i = 1; i <= stackCount - 1; ++i)
	{
		float phi = i*phiStep;

		// Vertices of ring.
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			float theta = j*thetaStep;

			VertexTypeShader3D v;

			// spherical to cartesian
			v.position.x = radius*sinf(phi)*cosf(theta);
			v.position.y = radius*cosf(phi);
			v.position.z = radius*sinf(phi)*sinf(theta);

			DirectX::XMFLOAT3 tmp = { v.position.x, v.position.y, v.position.z };

			DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&tmp);
			DirectX::XMStoreFloat3(&v.normal, DirectX::XMVector3Normalize(p));

			v.texture.x = theta / DirectX::XM_2PI;
			v.texture.y = phi / DirectX::XM_PI;

			cVertices.push_back(v);
		}
	}

	cVertices.push_back(bottomVertex);

	//
	// Compute indices for top stack.  The top stack was written first to the vertex buffer
	// and connects the top pole to the first ring.
	//

	for (UINT i = 1; i <= sliceCount; ++i)
	{
		cIndices.push_back(0);
		cIndices.push_back(i + 1);
		cIndices.push_back(i);
	}

	//
	// Compute indices for inner stacks (not connected to poles).
	//

	// Offset the indices to the index of the first vertex in the first ring.
	// This is just skipping the top pole vertex.
	UINT baseIndex = 1;
	UINT ringVertexCount = sliceCount + 1;
	for (UINT i = 0; i < stackCount - 2; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			cIndices.push_back(baseIndex + i*ringVertexCount + j);
			cIndices.push_back(baseIndex + i*ringVertexCount + j + 1);
			cIndices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			
			cIndices.push_back(baseIndex + (i + 1)*ringVertexCount + j);
			cIndices.push_back(baseIndex + i*ringVertexCount + j + 1);
			cIndices.push_back(baseIndex + (i + 1)*ringVertexCount + j + 1);
		}
	}

	//
	// Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
	// and connects the bottom pole to the bottom ring.
	//

	// South pole vertex was added last.
	UINT southPoleIndex = (UINT)cVertices.size() - 1;

	// Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		cIndices.push_back(southPoleIndex);
		cIndices.push_back(baseIndex + i);
		cIndices.push_back(baseIndex + i + 1);
	}
}