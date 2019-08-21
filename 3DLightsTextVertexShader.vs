struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular; // w = SpecPower
	float4 reflect;
};

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix orthoMatrix;
	Material bMaterial;
};

struct VertexInputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
	float4 positionH : SV_POSITION;
	float4 positionW : POSITIONW;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

PixelInputType TextureVertexLightsShader3D(VertexInputType input)
{
	PixelInputType output;
	
	input.position.w = 1.0f;
	
	output.positionH = mul(input.position, worldMatrix);
    output.positionH = mul(output.positionH, viewMatrix);
    output.positionH = mul(output.positionH, orthoMatrix);

	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);

	
	output.positionW = mul(input.position, worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

	return output;
}