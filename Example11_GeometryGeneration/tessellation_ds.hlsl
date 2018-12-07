// Tessellation domain shader
// After tessellation the domain shader processes the all the vertices
Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

struct ConstantOutputType
{
	float edges[4] : SV_TessFactor;
	float inside[2] : SV_InsideTessFactor;
};

struct InputType
{
	float3 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct OutputType
{
	float4 position : POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;

};

cbuffer TimeBuffer : register(b1)
{
	float4 time;
	float2 amplitude;
	float2 angularWave;
	float2 angularFrequency;
	float2 phaseShift;
};



float sumOfWaves(float x, float z)
{
	float distance = sqrt(x*x + z * z);

	float sum = 0;
	float sinIn1 = angularWave.x * distance - time.x * angularFrequency.x + phaseShift.x;
	float sinIn2 = angularWave.y * distance - time.x * angularFrequency.y + phaseShift.y;
	sum = (amplitude.x * sin(sinIn1)) + (amplitude.y * sin(sinIn2));
	//sum += amplitude.y * cos(sinIn2);

	return sum;
}
float getnormal(float x, float z)
{
	float distance = sqrt(x*x + z * z);

	float sum = 0;
	float sinIn1 = angularWave.x * distance - time.x * angularFrequency.x + phaseShift.x;
	float sinIn2 = angularWave.y * distance - time.x * angularFrequency.y + phaseShift.y;

	float normal;
	normal = amplitude.x * cos(sinIn1) + amplitude.y * cos(sinIn2);
	return normal;
}

[domain("quad")]
OutputType main(ConstantOutputType input, float2 uvwCoord : SV_DomainLocation, const OutputPatch<InputType, 4> patch)
{
	float3 vertexPosition;
	OutputType output;


	//calculate the actual position of the vertex
	float3 v1 = lerp(patch[0].position, patch[1].position, uvwCoord.y);
	float3 v2 = lerp(patch[3].position, patch[2].position, uvwCoord.y);
	vertexPosition = lerp(v1, v2, uvwCoord.x);

	//calculate the normal 
	float3 test1 = lerp(patch[0].normal, patch[1].normal, uvwCoord.y);
	float3 test2 = lerp(patch[3].normal, patch[2].normal, uvwCoord.y);
	output.normal = lerp(test1, test2, uvwCoord.x);

	float2 tex1 = lerp(patch[0].tex, patch[1].tex, uvwCoord.y);
	float2 tex2 = lerp(patch[3].tex, patch[2].tex, uvwCoord.y);
	output.tex = lerp(tex1, tex2, uvwCoord.x);

	//output.normal = mul(output.normal, (float3x3)worldMatrix);

	//// Normalize the normal vector.
	//output.normal = normalize(output.normal);



	float4 colour = texture0.SampleLevel(sampler0, output.tex, 0, 0);

	//vertexPosition += output.normal * colour * 5;


	//base height on the coulour times by 10 so we can actualy see it 


	////this is to limit the madness

	 //this checks what face we are applying the wave to by checking which way the normal is facing
	if (output.normal.y == 1)
	{
		vertexPosition.y = sumOfWaves(vertexPosition.x, vertexPosition.z);
		output.normal.x = 1 - getnormal(vertexPosition.x, vertexPosition.z);
		output.normal.y = abs(output.normal.x);
	}
	else if (output.normal.y == -1)
	{
		vertexPosition.y = -sumOfWaves(vertexPosition.x, vertexPosition.z);
		output.normal.x = (1 - getnormal(vertexPosition.x, vertexPosition.z));
		output.normal.y = -abs(output.normal.x);
	}

	else if (output.normal.x == 1)
	{
		vertexPosition.x = sumOfWaves(vertexPosition.y, vertexPosition.z);
		output.normal.z = (1 - getnormal(vertexPosition.y, vertexPosition.z));
		output.normal.x = abs(output.normal.x);
	}
	else if (output.normal.x == -1)
	{
		vertexPosition.x = -sumOfWaves(vertexPosition.y, vertexPosition.z);
		output.normal.z = (1 - getnormal(vertexPosition.y, vertexPosition.z));
		output.normal.x = -abs(output.normal.x);
	}

	else if (output.normal.z == 1)
	{
		vertexPosition.z = sumOfWaves(vertexPosition.x, vertexPosition.y);
		output.normal.x = (1 - getnormal(vertexPosition.x, vertexPosition.y));
		output.normal.z = abs(output.normal.x);
	}
	else if (output.normal.z == -1)
	{
		vertexPosition.z = -sumOfWaves(vertexPosition.x, vertexPosition.y);
		output.normal.x = (1 - getnormal(vertexPosition.x, vertexPosition.y));
		output.normal.y = -abs(output.normal.x);
	}



	//vertexPosition.y = sumOfWaves(vertexPosition.x, vertexPosition.z);

	// Calculate the position of the new vertex against the world, view, and projection matrices.
	output.position = float4(vertexPosition, 1.0f); // mul(float4(vertexPosition, 1.0f), worldMatrix);
													/*  output.position = mul(output.position, viewMatrix);
													output.position = mul(output.position, projectionMatrix);

													output.worldPosition = mul(float4(vertexPosition, 1.0f), worldMatrix).xyz;*/

	return output;
}

