#pragma once
#include "GraphicsDxd.h"
#include <vector>

class Sphere
{
public:
	Sphere(float radius, UINT sliceCount, UINT stackCount, ID3D11Device* pDevice);
	Sphere() = delete;
	Sphere(const Cube& quad) = delete;
	~Sphere();

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;
	unsigned int GetIndexCount() const;
private:
	void CreateSphere(float radius, UINT sliceCount, UINT stackCount);

private:
	ID3D11Buffer *cPVertexBuffer, *cPIndexBuffer;
	unsigned int cVertexCount, cIndexCount;

	std::vector<VertexTypeShader3D> cVertices;
	std::vector<DWORD> cIndices;
};