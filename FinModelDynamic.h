#pragma once
#include "GraphicsDxd.h"

//        |\        |
//  F     | \       |
//  O     |  \      | m
//  G     |   \     |
//  U  cr |\   \    |
//  E     | \   \   |
//  T     |  \l  |
//  E     |   \  |
//         \   \ |         
//          \   \|           
//           \   | ct       
//            \  |          
//             \ |            
//              \|                
//        --------
//            s

class FinModelDynamic
{
public:
	//Olhar FinModel.h para detalhes de cada parametro.
	FinModelDynamic(float chordRoot, float chordTrailling, float m, float s, float thickness, ID3D11Device* pDevice);
	FinModelDynamic() = delete;
	FinModelDynamic(const Cube& quad) = delete;
	~FinModelDynamic();

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;
	unsigned int GetIndexCount() const;

	void ChangeFin(float cordaRaiz, float cordaPonta, float deslCorda, float envergadura,
		float espessura, GraphicsDxD* pGfx);
private:
	ID3D11Buffer *cPVertexBuffer, *cPIndexBuffer;
	unsigned int cVertexCount, cIndexCount;
};