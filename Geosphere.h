#pragma once
#include "GraphicsDxd.h"
#include <vector>

class Geosphere
{
public:
	Geosphere(float radius, UINT numSubdivisions, ID3D11Device* pDevice);
	Geosphere() = delete;
	Geosphere(const Cube& quad) = delete;
	~Geosphere();

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;
	unsigned int GetIndexCount() const;
private:
	void CreateGeosphere(float radius, UINT numSubdivisions);
	void Geosphere::Subdivide();

private:
	ID3D11Buffer *cPVertexBuffer, *cPIndexBuffer;
	unsigned int cVertexCount, cIndexCount;

	std::vector<VertexTypeShader3D> cVertices;
	std::vector<DWORD> cIndices;
};