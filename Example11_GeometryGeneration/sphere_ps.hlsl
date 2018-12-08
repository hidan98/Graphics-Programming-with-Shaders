
Texture2D texture0 : register(t0);
Texture2D textureBump : register(t1);
SamplerState Sampler0 : register(s0);

Texture2D depthMapTexture : register(t2);
Texture2D depthMapTexture1 : register(t3);

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

	float depthValue;
	float lightDepthValue;
	float shadowMapBias = 0.005f;
	float4 colour = float4(0.f, 0.f, 0.f, 1.f);

	colour = calculateLighting(-direction[0], newNormal, diffuse[0]);


	//// Calculate the projected texture coordinates.
	//float2 pTexCoord = input.lightViewPos[0].xy / input.lightViewPos[0].w;
	//pTexCoord *= float2(0.5, -0.5);
	//pTexCoord += float2(0.5f, 0.5f);

	//// Determine if the projected coordinates are in the 0 to 1 range.  If not don't do lighting.
	//if (pTexCoord.x < 0.f || pTexCoord.x > 1.f || pTexCoord.y < 0.f || pTexCoord.y > 1.f)
	//{
	//	colour += calculateLighting(-direction[0], newNormal, diffuse[0]);
	//	//return textureColour;
	//}

	//// Sample the shadow map (get depth of geometry)
	//depthValue = depthMapTexture.Sample(Sampler0, pTexCoord).r;
	//// Calculate the depth from the light.
	//lightDepthValue = input.lightViewPos[0].z / input.lightViewPos[0].w;
	//lightDepthValue -= shadowMapBias;

	//// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
	//if (lightDepthValue < depthValue)
	//{

	//	colour = calculateLighting(-direction[0], newNormal, diffuse[0]);
	//}

	colour = saturate(colour + ambient[0]);


	//// Calculate the projected texture coordinates.
	//pTexCoord = input.lightViewPos[1].xy / input.lightViewPos[1].w;
	//pTexCoord *= float2(0.5, -0.5);
	//pTexCoord += float2(0.5f, 0.5f);

	//// Determine if the projected coordinates are in the 0 to 1 range.  If not don't do lighting.
	//if (pTexCoord.x < 0.f || pTexCoord.x > 1.f || pTexCoord.y < 0.f || pTexCoord.y > 1.f)
	//{
	//	colour += calculateLighting(-direction[1], newNormal, diffuse[1]);
	//	//return textureColour;
	//}

	//// Sample the shadow map (get depth of geometry)
	//depthValue = depthMapTexture1.Sample(Sampler0, pTexCoord).r;
	//// Calculate the depth from the light.
	//lightDepthValue = input.lightViewPos[1].z / input.lightViewPos[1].w;
	//lightDepthValue -= shadowMapBias;

	//// Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel.
	//if (lightDepthValue < depthValue)
	//{
	//	colour += calculateLighting(-direction[1], newNormal, diffuse[1]);
	//}

	//colour = saturate(colour + ambient[0]);
	return saturate(colour) * textureColour;

}