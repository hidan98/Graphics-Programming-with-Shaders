// Tessellation pixel shader
// Output colour passed to stage.
Texture2D texture0 : register(t0);
SamplerState Sampler0 : register(s0);

struct InputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;

};

cbuffer LightBuffer : register(b0)
{
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

	float4 textureColour;
	float4 lightColour;
	float4 BrightColour;
	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	textureColour = texture0.Sample(Sampler0, input.tex);
	//lightColour = calculateLighting(-lightDirection, input.normal, diffuseColour);

	lightColour = textureColour;

	float brightness = dot(lightColour.rgb, float3(0.2126, 0.7152, 0.0722));
	if (brightness > 0.7f)
	{
		return BrightColour = float4(lightColour.rgb, 1.0);
	}
	return BrightColour = float4(0.0, 0.0, 0.0, 1.0);

}