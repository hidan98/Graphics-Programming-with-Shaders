cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct InputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 tangent : TANGENT;
};

struct OutputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 tangent : TANGENT;
};


OutputType main(InputType input)
{
	OutputType output;

	output.position = input.position;
	output.normal = input.normal;
	output.tex = input.tex;
	output.tangent = input.tangent;
		/*float3 tan1 = lerp(patch[0].tangent, patch[1].tangent, uvwCoord.y);
	float3 tan2 = lerp(patch[3].tangent, patch[2].tangent, uvwCoord.y);
	output.tangent = lerp(tan1, tan1, uvwCoord.x);	

	output.tangent = mul(output.tangent, (float3x3)worldMatrix);
	output.tangent = normalize(output.tangent);*/

	return output;
}