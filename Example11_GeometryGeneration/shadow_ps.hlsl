
Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture : register(t1);
Texture2D depthMapTexture1 : register(t2);

SamplerState diffuseSampler  : register(s0);
SamplerState shadowSampler : register(s1);

cbuffer LightBuffer : register(b0)
{
	float4 ambient[2];
	float4 diffuse[2];
	float4 direction[2];
};

struct InputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float4 lightViewPos[2] : TEXCOORD1;
};

// Calculate lighting intensity based on direction and normal. Combine with light colour.
float4 calculateLighting(float3 lightDirection, float3 normal, float4 diffuse)
{
	float intensity = saturate(dot(normal, lightDirection));
	float4 colour = saturate(diffuse * intensity);
	return colour;
}

float4 dostuff(float4 lightPos, float3 lightDirrection, float4 diffuse, float3 normal, float4 ambientLight, Texture2D depth, float4 textureColour)
{
	
	float lightDepthValue;
	float shadowMapBias = 0.005f;


	float4 colour = float4(0.f, 0.f, 0.f, 1.f);
	// Calculate the projected texture coordinates.
	float2 pTexCoord = lightPos.xy / lightPos.w;
	pTexCoord *= float2(0.5, -0.5);
	pTexCoord += float2(0.5f, 0.5f);

	// Determine if the projected coordinates are in the 0 to 1 range.  If not don't do lighting.
	if (pTexCoord.x < 0.f || pTexCoord.x > 1.f || pTexCoord.y < 0.f || pTexCoord.y > 1.f)
	{	
		return textureColour;
	}

	float depthValue = depth.Sample(shadowSampler, pTexCoord).r;
		
	// Calculate the depth from the light.
	lightDepthValue = lightPos.z / lightPos.w;
	lightDepthValue -= shadowMapBias;

	// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
	if ((lightDepthValue < depthValue))
	{
		//return float4(0.0f, 0.0f, 0.0f, 1.0f);
		colour = calculateLighting(-lightDirrection, normal, diffuse);
		return colour = saturate(colour + ambientLight);

	}
	//return float4(1.0f, 0.0f, 0.0f, 1.0f);
	return float4(0.0f, 0.0f, 0.0f, 1.0f);
	return colour = saturate(colour + ambientLight);
}


float4 main(InputType input) : SV_TARGET
{
	
	float4 textureColour = shaderTexture.Sample(diffuseSampler, input.tex);

	float4 colour = float4(0.f, 0.f, 0.f, 1.f);
	colour += dostuff(input.lightViewPos[0], direction[0], diffuse[0], input.normal[0], ambient[0], depthMapTexture, textureColour);
	colour += dostuff(input.lightViewPos[1], direction[1], diffuse[1], input.normal[1], ambient[1], depthMapTexture1, textureColour);

		
	
	return colour * textureColour;
}