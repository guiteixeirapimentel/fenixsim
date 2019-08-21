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

class FinModel
{
public:
	//Olhar FinModel.h para detalhes de cada parametro.
	FinModel(float chordRoot, float chordTrailling, float m, float s, float thickness, ID3D11Device* pDevice);
	FinModel() = delete;
	FinModel(const Cube& quad) = delete;
	~FinModel();

	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;
	unsigned int GetIndexCount() const;
private:
	ID3D11Buffer *cPVertexBuffer, *cPIndexBuffer;
	unsigned int cVertexCount, cIndexCount;
};