// Tessellation Hull Shader
// Prepares control points for tessellation
struct InputType
{
    float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct ConstantOutputType
{
	float edges[4] : SV_TessFactor;
	float inside[2] : SV_InsideTessFactor;
};

struct OutputType
{
    float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

cbuffer CamBuffer : register(b0)
{
	float3 camPos;
	float padding;
}

ConstantOutputType PatchConstantFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{    
    ConstantOutputType output;

	float3 center = (inputPatch[0].position + inputPatch[1].position + inputPatch[2].position + inputPatch[3].position) / 4;
	float d = distance(center, camPos);

	//this is the tesselation scale
	float t = 1.f - ((d - 1.f) / 20.f - 1.f);
	//this is the tesselation factor
	t = (0.4f *t) * 63.f + 1.f;
	//just make sure that it is in the correct range between 1- 64;
	float tessFac1 = clamp(t, 1.f, 64.f);

	float tesselation = 64;
	//uses the calculated tessilation factor 
	output.edges[0] = tessFac1;
	output.edges[1] = tessFac1;
	output.edges[2] = tessFac1;
	output.edges[3] = tessFac1;

	output.inside[0] = tessFac1;
	output.inside[1] = tessFac1;

	
	return output;
}


[domain("quad")]
[partitioning("integer")]
[outputtopology("triangle_ccw")]
[outputcontrolpoints(4)]
[patchconstantfunc("PatchConstantFunction")]
OutputType main(InputPatch<InputType, 4> patch, uint pointId : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
    OutputType output;
	//repack the data  
    output.position = patch[pointId].position;
	output.tex = patch[pointId].tex;
	output.normal = patch[pointId].normal;
    

    return output;
}