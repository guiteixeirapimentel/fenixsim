cbuffer MatrixBuffer
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix orthoMatrix;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

PixelInputType TextureVertexShader2D(VertexInputType input)
{
	PixelInputType output;
	
	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);	
    output.position = mul(output.position, orthoMatrix);

	output.tex = input.tex;

	return output;
}