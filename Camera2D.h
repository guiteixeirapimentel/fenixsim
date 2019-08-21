#pragma once
#include <DirectXMath.h>

class Camera2D
{
public:
	Camera2D();
	~Camera2D();

	void SetPosition(float x, float y);
	void SetRotation(float alpha);

	DirectX::XMFLOAT2 GetPosition() const;
	float GetRotation() const;

	void Render();
	void GetViewMatrix(DirectX::XMMATRIX& out) const;

private:
	DirectX::XMFLOAT2 cPosition;
	float cAlpha;
	DirectX::XMMATRIX cViewMatrix;
};
