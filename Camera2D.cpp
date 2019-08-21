#include "Camera2D.h"

Camera2D::Camera2D()
	:
	cPosition({ 0.0, 0.0 }),
	cAlpha(0.0)
{
	cViewMatrix = DirectX::XMMatrixRotationZ(cAlpha) * DirectX::XMMatrixTranslation(cPosition.x, cPosition.y, 0.0);
}

Camera2D::~Camera2D()
{}

void Camera2D::SetPosition(float x, float y)
{
	cPosition.x = x;
	cPosition.y = y;
}

void Camera2D::SetRotation(float alpha)
{
	cAlpha = alpha;
}

DirectX::XMFLOAT2 Camera2D::GetPosition() const
{
	return cPosition;
}

float Camera2D::GetRotation() const
{
	return cAlpha;
}

void Camera2D::Render()
{
	cViewMatrix = DirectX::XMMatrixRotationZ(cAlpha) * DirectX::XMMatrixTranslation(cPosition.x, cPosition.y, 0.0);
}

void Camera2D::GetViewMatrix(DirectX::XMMATRIX& out) const
{
	out = cViewMatrix;
}