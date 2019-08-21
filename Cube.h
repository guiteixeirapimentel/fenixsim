#pragma once
#include "GraphicsDxd.h"

class Cube
{
public:
	Cube(ID3D11Device* pDevice);
	Cube() = delete;
	Cube(const Cube& quad) = delete;
	~Cube();

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;
	unsigned int GetIndexCount() const;
private:
	ID3D11Buffer *cPVertexBuffer, *cPIndexBuffer;
	unsigned int cVertexCount, cIndexCount;
};