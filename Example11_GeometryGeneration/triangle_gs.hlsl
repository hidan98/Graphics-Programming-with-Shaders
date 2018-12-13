cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
	
};

cbuffer lightMatrixBuffer : register(b2)
{
	matrix lightViewMatrix[2];
	matrix lightProjectionMatrix[2];
}

cbuffer TimeBuffer : register(b1)
{
	float time;
	float3 padding;
};

struct InputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;	
	float4 depthPosition : TEXCOORD3;
	float4 lightViewPos[2] : TEXCOORD1;
};

OutputType explode(InputType input, float normal)
{
	OutputType output;

	//the distance/ direction is along the normalwith the ofset of sin(time/5) to slow it down + the direction
	float3 position =  normal * sin(time / 5) + input.position;

	output.position = float4(input.position, 1.0f) + float4(position, 1.0f);
	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);


	// Calculate the position of the vertice as viewed by the light source.
	for (int i = 0; i < 2; i++)
	{
		output.lightViewPos[i] = mul(input.position, worldMatrix);
		output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
		output.lightViewPos[i]= mul(output.lightViewPos[i], lightProjectionMatrix[i]);
	}
	

	output.tex = input.tex;

	output.normal = normal;

	output.depthPosition = output.position;
	return output;
}
[maxvertexcount(3)]
void main(triangle InputType input[3], inout TriangleStream< OutputType > triStream)
{
	OutputType output;
	
	output.normal = mul(input[0].normal, (float3x3) worldMatrix);
	output.normal = normalize(output.normal);
	for (int i = 0; i < 3; i++)
	{
		output = explode(input[i], output.normal);
		triStream.Append(output);
	}
	triStream.RestartStrip();
}