// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct ConstantOutputType
{
	float edges[4] : SV_TessFactor;
	float inside[2] : SV_InsideTessFactor;
};

struct InputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;

};

[domain("quad")]
OutputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
	float3 vertexPosition;
	OutputType output;

	//calculate the actual position of the vertex
	float3 v1 = lerp(patch[0].position, patch[1].position, uvwCoord.y);
	float3 v2 = lerp(patch[3].position, patch[2].position, uvwCoord.y);
	vertexPosition = lerp(v1, v2, uvwCoord.x);
	
	// Calculate the position of the vertex against the world, view, and projection matrices.
	

	//calculate the normal 
	float3 test1 = lerp(patch[0].normal, patch[1].normal, uvwCoord.y);
	float3 test2 = lerp(patch[3].normal, patch[2].normal, uvwCoord.y);
	output.normal = lerp(test1, test2, uvwCoord.x);

	output.normal = mul(output.normal, (float3x3)worldMatrix);
	output.normal = normalize(output.normal);

	float2 tex1 = lerp(patch[0].tex, patch[1].tex, uvwCoord.y);
	float2 tex2 = lerp(patch[3].tex, patch[2].tex, uvwCoord.y);
	output.tex = lerp(tex1, tex2, uvwCoord.x);

	

	float3 tan1 = lerp(patch[0].tangent, patch[1].tangent, uvwCoord.y);
	float3 tan2 = lerp(patch[3].tangent, patch[2].tangent, uvwCoord.y);
	output.tangent = lerp(tan1, tan1, uvwCoord.x);	

	output.tangent = mul(output.tangent, (float3x3)worldMatrix);
	output.tangent = normalize(output.tangent);
	

	vertexPosition += output.normal * texture0.SampleLevel(sampler0, output.tex, 0, 0) * 2;



	output.position = mul(float4(vertexPosition, 1.0f), worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);
	return output;
}

