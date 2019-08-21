#pragma once
#include "GraphicsDxd.h"
class Quad
{
public:
	Quad(ID3D11Device* pDevice);
	Quad() = delete;
	Quad(const Quad& quad) = delete;
	~Quad();

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;
	unsigned int GetIndexCount() const;
private:
	ID3D11Buffer *cPVertexBuffer, *cPIndexBuffer;
	unsigned int cVertexCount, cIndexCount;
};