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
	float4 ambiantColour;
	float4 diffuseColour;
	float3 lightDirection;
	float padding;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}

float4 main(InputType input) : SV_TARGET
{
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.

	float4 textureColour = texture0.Sample(Sampler0, input.tex);

	float4 lightColour = calculateLighting(-lightDirection, input.normal, diffuseColour);

	//lightColour = saturate(lightColour + ambiantColour);

	return textureColour;
	//return float4 (texture0.Sample(Sampler0, input.tex));
	
}