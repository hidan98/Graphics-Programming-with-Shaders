
Texture2D texture0 : register(t0);
Texture2D textureBump : register(t1);
SamplerState Sampler0 : register(s0);

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};
cbuffer LightBuffer : register(b0)
{
	float4 ambiantColour;
	float4 diffuseColour;
	float3 lightDirection;
	float padding;
};


float3 reCalculateNormal(float3 normalMap, float3 normal, float3 tangent)
{
	float3 noramlTan = (2.0f*normalMap) - 1.0f;

	//build componants of tbn
	float3 N = normal;
	float3 T = normalize(tangent - dot(tangent, N) * N);
	float3 B = cross(N, T);

	float3x3 TBN = float3x3(T, B, N);

	float3 bumpNormal = mul(noramlTan, TBN);

	return normalize(bumpNormal);
}

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}


float4 main(InputType input) : SV_TARGET
{
	float3 normalMap = textureBump.Sample(Sampler0, input.tex).rgb;	

	float3 newNormal = reCalculateNormal(normalMap, input.normal, input.tangent);

	float4 textureColour = texture0.Sample(Sampler0, input.tex);

	float4 lightColour = calculateLighting(-lightDirection, newNormal, diffuseColour);

	lightColour = saturate(lightColour + ambiantColour);

	lightColour *= textureColour;
	return lightColour;

}