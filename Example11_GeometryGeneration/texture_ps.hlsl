// Texture pixel/fragment shader
// Basic fragment shader for rendering textured geometry

// Texture and sampler registers
Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};


cbuffer LightBuffer : register(b0)
{
	float4 diffuseColour;
	float3 lightDirection;
	bool renderNormal;
};
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	//float4 lighColour = calculateLighting(-lightDirection, input.normal, diffuseColour);
	float4 textureColour = texture0.Sample(Sampler0, input.tex);
	
	return textureColour;// *lighColour;
	
	
}