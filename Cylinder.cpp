#include "Cylinder.h"
#include "Shader3D.h"

Cylinder::Cylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, ID3D11Device* pDevice)
	:
	cPIndexBuffer(nullptr),
	cPVertexBuffer(nullptr)
{
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;


	CreateCylinder(bottomRadius, topRadius, height, sliceCount, stackCount);

		
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

Cylinder::~Cylinder()
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

ID3D11Buffer* Cylinder::GetVertexBuffer() const
{
	return cPVertexBuffer;
}

ID3D11Buffer* Cylinder::GetIndexBuffer() const
{
	return cPIndexBuffer;
}

unsigned int Cylinder::GetIndexCount() const
{
	return cIndexCount;
}

void Cylinder::CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount)
{
	cVertices.clear();
	cIndices.clear();

	//
	// Build Stacks.
	// 

	float stackHeight = height / stackCount;

	// Amount to increment radius as we move up each stack level from bottom to top.
	float radiusStep = (topRadius - bottomRadius) / stackCount;

	UINT ringCount = stackCount + 1;
	
	// Compute vertices for each stack ring starting at the bottom and moving up.
	for (UINT i = 0; i < ringCount; ++i)
	{
		float y = -0.5f*height + i*stackHeight;
		float r = bottomRadius + i*radiusStep;

		// vertices of ring
		float dTheta = 2.0f*DirectX::XM_PI / sliceCount;
		for (UINT j = 0; j <= sliceCount; ++j)
		{
			VertexTypeShader3D vertex;

			float c = cosf(j*dTheta);
			float s = sinf(j*dTheta);

			vertex.position = DirectX::XMFLOAT4(r*c, y, r*s, 1.0f);

			vertex.texture.x = (float)j / sliceCount;
			vertex.texture.y = 1.0f - (float)i / stackCount;

			// Cylinder can be parameterized as follows, where we introduce v
			// parameter that goes in the same direction as the v tex-coord
			// so that the bitangent goes in the same direction as the v tex-coord.
			//   Let r0 be the bottom radius and let r1 be the top radius.
			//   y(v) = h - hv for v in [0,1].
			//   r(v) = r1 + (r0-r1)v
			//
			//   x(t, v) = r(v)*cos(t)
			//   y(t, v) = h - hv
			//   z(t, v) = r(v)*sin(t)
			// 
			//  dx/dt = -r(v)*sin(t)
			//  dy/dt = 0
			//  dz/dt = +r(v)*cos(t)
			//
			//  dx/dv = (r0-r1)*cos(t)
			//  dy/dv = -h
			//  dz/dv = (r0-r1)*sin(t)

			// This is unit length.
			auto TangentU = DirectX::XMFLOAT3(-s, 0.0f, c);

			float dr = bottomRadius - topRadius;
			DirectX::XMFLOAT3 bitangent(dr*c, -height, dr*s);

			DirectX::XMVECTOR T = DirectX::XMLoadFloat3(&TangentU);
			DirectX::XMVECTOR B = DirectX::XMLoadFloat3(&bitangent);
			DirectX::XMVECTOR N = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(T, B));
			DirectX::XMStoreFloat3(&vertex.normal, N);

			cVertices.emplace_back(vertex);
		}
	}

	// Add one because we duplicate the first and last vertex per ring
	// since the texture coordinates are different.
	UINT ringVertexCount = sliceCount + 1;
	UINT nI = 0;
	// Compute indices for each stack.

	for (UINT i = 0; i < stackCount; ++i)
	{
		for (UINT j = 0; j < sliceCount; ++j)
		{
			cIndices.push_back(i*ringVertexCount + j);
			cIndices.push_back((i + 1)*ringVertexCount + j);
			cIndices.push_back((i + 1)*ringVertexCount + j + 1);
			cIndices.push_back(i*ringVertexCount + j);
			cIndices.push_back((i + 1)*ringVertexCount + j + 1);
			cIndices.push_back(i*ringVertexCount + j + 1);
		}
	}

	BuildCylinderTopCap(bottomRadius, topRadius, height, sliceCount, stackCount);
	BuildCylinderBottomCap(bottomRadius, topRadius, height, sliceCount, stackCount);
}

void Cylinder::BuildCylinderTopCap(float bottomRadius, float topRadius, float height,
	UINT sliceCount, UINT stackCount)
{
	UINT baseIndex = (UINT)cVertices.size();

	float y = 0.5f*height;
	float dTheta = 2.0f*DirectX::XM_PI / sliceCount;

	// Duplicate cap ring vertices because the texture coordinates and normals differ.
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = topRadius*cosf(i*dTheta);
		float z = topRadius*sinf(i*dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		cVertices.push_back({ {x, y, z, 1.0f}, { u, v}, {0.0f, 1.0f, 0.0f} });
	}

	// Cap center vertex.
	cVertices.push_back({ { 0.0f, y, 0.0f, 1.0f},{ 0.5f, 0.5f }, {0.0f, 1.0f, 0.0f} });

	// Index of center vertex.
	UINT centerIndex = (UINT)cVertices.size() - 1;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		cIndices.push_back(centerIndex);
		cIndices.push_back(baseIndex + i + 1);
		cIndices.push_back(baseIndex + i);
	}
}

void Cylinder::BuildCylinderBottomCap(float bottomRadius, float topRadius, float height,
	UINT sliceCount, UINT stackCount)
{
	// 
	// Build bottom cap.
	//

	UINT baseIndex = (UINT)cVertices.size();
	float y = -0.5f*height;

	// vertices of ring
	float dTheta = 2.0f*DirectX::XM_PI / sliceCount;
	for (UINT i = 0; i <= sliceCount; ++i)
	{
		float x = bottomRadius*cosf(i*dTheta);
		float z = bottomRadius*sinf(i*dTheta);

		// Scale down by the height to try and make top cap texture coord area
		// proportional to base.
		float u = x / height + 0.5f;
		float v = z / height + 0.5f;

		cVertices.push_back({ { x, y, z, 1.0f }, { u, v }, { 0.0f, -1.0f, 0.0f }});
	}

	// Cap center vertex.
	cVertices.push_back({ {0.0f, y, 0.0f, 1.0f},{ 0.5f, 0.5f }, {0.0f, -1.0f, 0.0f} });

	// Cache the index of center vertex.
	UINT centerIndex = (UINT)cVertices.size() - 1;

	for (UINT i = 0; i < sliceCount; ++i)
	{
		cIndices.push_back(centerIndex);
		cIndices.push_back(baseIndex + i);
		cIndices.push_back(baseIndex + i + 1);
	}
}
