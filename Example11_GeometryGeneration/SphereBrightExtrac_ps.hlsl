Texture2D texture0 : register(t0);
Texture2D textureBump : register(t1);
SamplerState Sampler0 : register(s0);


struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4 lightViewPos[2] : TEXCOORD1;
};
cbuffer LightBuffer : register(b0)
{
	float4 ambient[2];
	float4 diffuse[2];
	float4 direction[2];
};

cbuffer brightBuffer : register(b1)
{
	float bright;
	float3 padding;
}

float4 brightTest(float4 colour)
{
	float4 returnBright;

	float Pixelbrightness = dot(colour.rgb, float3(0.2126, 0.7152, 0.0722));
	if (Pixelbrightness > bright)
	{
		return returnBright = float4(colour.rgb, 1.0);
	}
	return returnBright = float4(0.0, 0.0, 0.0, 1.0);
}

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}

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


float4 main(InputType input) : SV_TARGET
{

	float3 normalMap = textureBump.Sample(Sampler0, input.tex).rgb;

	float3 newNormal = reCalculateNormal(normalMap, input.normal, input.tangent);

	float4 colour = float4(0.f, 0.f, 0.f, 1.f);
	float4 textureColour;
	float4 returnBright;
	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	textureColour = texture0.Sample(Sampler0, input.tex);
	for (int i = 0; i < 2; i++)
	{

		colour += calculateLighting(-direction[i], newNormal, diffuse[i]);

		colour = saturate(colour + ambient[i]);
	}

	colour *= textureColour;


	return brightTest(colour);

}