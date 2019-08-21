#pragma once
#include "GraphicsDxd.h"

class Parallelepiped
{
public:
	Parallelepiped(float widthBottom, float widthTop, float heightFront, float heightBack,
		float depthBottom, float depthTop, ID3D11Device* pDevice);
	Parallelepiped() = delete;
	Parallelepiped(const Cube& quad) = delete;
	~Parallelepiped();

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;
	unsigned int GetIndexCount() const;
private:
	ID3D11Buffer *cPVertexBuffer, *cPIndexBuffer;
	unsigned int cVertexCount, cIndexCount;
};