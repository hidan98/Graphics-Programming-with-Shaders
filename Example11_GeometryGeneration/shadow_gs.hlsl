cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;

};
cbuffer TimeBuffer : register(b1)
{
	float time;
	float3 padding;
};

cbuffer lightMatrixBuffer : register(b2)
{
	matrix lightViewMatrix[2];
	matrix lightProjectionMatrix[2];
}

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
	float4 lightViewPos[2] : TEXCOORD1;
};

OutputType explode(InputType input, float normal)
{
	OutputType output;

	//float3 normal = mul(input.normal, (float3x3)worldMatrix);
	//normal = normalize(normal);

	float magnitude = 2.f;

	float3 direction = normal * sin(time / 5) + input.position;

<<<<<<< HEAD
<<<<<<< HEAD
	output.position = float4(input.position, 1.0f) +float4(direction, 1.0f);
=======
	output.position = float4(input.position, 1.0f) + float4(direction, 1.0f);
>>>>>>> parent of 6143710... bump mapping works!!!!
=======
	output.position = float4(input.position, 1.0f) + float4(direction, 1.0f);
>>>>>>> parent of 6143710... bump mapping works!!!!
	output.position = mul(output.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);


	// Calculate the position of the vertice as viewed by the light source.
	for (int i = 0; i < 2; i++)
	{
		output.lightViewPos[i] = mul(input.position, worldMatrix);
		output.lightViewPos[i] = mul(output.lightViewPos[i], lightViewMatrix[i]);
		output.lightViewPos[i] = mul(output.lightViewPos[i], lightProjectionMatrix[i]);
	}


	output.tex = input.tex;

	output.normal = normal;

	return output;
}

[maxvertexcount(3)]
void main(triangle InputType input[3], inout TriangleStream< OutputType > triStream)
{
	OutputType output;
	float3 a = input[0].position.xyz - input[1].position.xyz;
	float3 b = input[2].position.xyz - input[1].position.xyz;
	float3 normal = normalize(cross(a, b));
	for (int i = 0; i < 3; i++)
	{
		output = explode(input[i], normal);
		triStream.Append(output);
	}
	triStream.RestartStrip();
}