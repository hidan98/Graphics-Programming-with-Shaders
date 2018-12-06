// Tessellation Hull Shader
// Prepares control points for tessellation
struct InputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};

struct OutputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
};


struct ConstantOutputType
{
	float edges[4] : SV_TessFactor;
	float inside[2] : SV_InsideTessFactor;
};


ConstantOutputType PatchConstantFunction(InputPatch<InputType, 4> inputPatch, uint patchId : SV_PrimitiveID)
{
	ConstantOutputType output;


	float tesselation = 1;
	//uses the calculated tessilation factor 
	output.edges[0] = tesselation;
	output.edges[1] = tesselation;
	output.edges[2] = tesselation;
	output.edges[3] = tesselation;

	output.inside[0] = tesselation;
	output.inside[1] = tesselation;


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

	// Set the position for this control point as the output position.
	output.position = patch[pointId].position;

	output.tex = patch[pointId].tex;
	output.normal = patch[pointId].normal;
	output.tangent = patch[pointId].tangent;

	return output;
}