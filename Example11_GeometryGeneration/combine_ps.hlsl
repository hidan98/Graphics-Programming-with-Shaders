// Tessellation pixel shader
// Output colour passed to stage.
Texture2D texture0 : register(t0);
Texture2D texture1 : register(t1);
SamplerState Sampler0 : register(s0);

struct InputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;

};



float4 main(InputType input) : SV_TARGET
{
	float4 textureColour;
	textureColour = texture0.Sample(Sampler0, input.tex);
	return textureColour += texture1.Sample(Sampler0, input.tex);

}