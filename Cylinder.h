#pragma once
#include "GraphicsDxd.h"
#include <vector>

class Cylinder
{
public:
	Cylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount, ID3D11Device* pDevice);
	Cylinder() = delete;
	Cylinder(const Cube& quad) = delete;
	~Cylinder();

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;
	unsigned int GetIndexCount() const;
private:
	void BuildCylinderBottomCap(float bottomRadius, float topRadius, float height,
		UINT sliceCount, UINT stackCount);
	void BuildCylinderTopCap(float bottomRadius, float topRadius, float height,
		UINT sliceCount, UINT stackCount);

	void CreateCylinder(float bottomRadius, float topRadius, float height, UINT sliceCount, UINT stackCount);

private:
	ID3D11Buffer *cPVertexBuffer, *cPIndexBuffer;
	unsigned int cVertexCount, cIndexCount;

	std::vector<VertexTypeShader3D> cVertices;
	std::vector<DWORD> cIndices;
};