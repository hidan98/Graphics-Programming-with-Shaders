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
	float4 ambient[2];
	float4 diffuse[2];
	float4 direction[2];
};

cbuffer brightBuffer : register(b1)
{
	float bright;
	float3 padding;
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
	float4 colour = float4(0.f, 0.f, 0.f, 1.f);
	float4 textureColour;
	float4 returnBright;
	// Sample the texture. Calculate light intensity and colour, return light*texture for final pixel colour.
	textureColour = texture0.Sample(Sampler0, input.tex);
	//loop twice for two light sources
	for (int i = 0; i < 2; i++)
	{
		//calculate lighting 
		colour += calculateLighting(-direction[i], input.normal, diffuse[i]);
		//make sure the result does not exceed 1
		colour = saturate(colour + ambient[i]);
	}
	
	colour *= textureColour;

	//transform to gray scale 
	float Pixelbrightness = dot(colour.rgb, float3(0.2126, 0.7152, 0.0722));
	//check if the brightnes exceeds threshold
	if (Pixelbrightness > bright)
	{
		return returnBright = float4(colour.rgb, 1.0);
	}
	//if light is not bright enough return black
	return returnBright = float4(0.0, 0.0, 0.0, 1.0);

}