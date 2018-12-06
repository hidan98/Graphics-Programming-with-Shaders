#include "CustomSphereMesh.h"
#include <vector>


CustomSphereMesh::CustomSphereMesh(ID3D11Device* device)
{
	
	initBuffers(device);
}


CustomSphereMesh::~CustomSphereMesh()
{
}

void CustomSphereMesh::sendData(ID3D11DeviceContext * deviceContext)
{

	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexTypeTangent);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);

}

void CustomSphereMesh::initBuffers(ID3D11Device * device)
{

	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	/*vertexCount = 24;
	indexCount = 24;*/

	//VertexType* vertices = new VertexType[vertexCount];
//	unsigned long* indices = new unsigned long[indexCount];


	float texInc = (1.f / seg);

	//set up temp vectors for verticies, normals, and texture coords
	std::vector<XMFLOAT3> sphereVer;
	std::vector<XMFLOAT2> sphereTex;
	std::vector<XMFLOAT3> sphereNorm;
	std::vector<XMFLOAT3> sphereTangent;
	std::vector<unsigned long> sphereIndices;

	//vecor to store the number of quads only one item will be stored but will allow me to just pass one item to the functio to render
	std::vector<float> count;

	//set up angle
	theta = 0;
	delta = 0;
	thetaInc = (2 * XM_PI) / seg;
	deltaInc = XM_PI / seg;

	tex.y = 0;
	//loops for the number of segments specified
	for (int i = 0; i < seg; i++)
	{
		tex.x = 0;
		//loop again for number of sgments
		for (int j = 0; j < seg; j++)
		{

			//set up new coords
			sphereIndices.push_back(sphereVer.size());
			vertexPos.x = radius * cos(theta)*sin(delta + deltaInc);
			vertexPos.y = radius * cos(delta + deltaInc);
			vertexPos.z = radius * sin(theta) * sin(delta + deltaInc);
			sphereVer.push_back(vertexPos);
			//set up norms
			sphereNorm.push_back(XMFLOAT3(vertexPos.x / radius, vertexPos.y / radius, vertexPos.z / radius));
			//set textures
			sphereTex.push_back(XMFLOAT2(tex.x, tex.y + texInc));

			tangent.x = -radius*sinf(theta)*sinf(delta +deltaInc);
			tangent.y = 0.0f;
			tangent.z = radius * sinf(theta) *cosf(delta + deltaInc);
			sphereTangent.push_back(tangent);



			//set up new coords
			sphereIndices.push_back(sphereVer.size());
			vertexPos.x = radius * cos(theta + thetaInc)*sin(delta + deltaInc);
			vertexPos.y = radius * cos(delta + deltaInc);
			vertexPos.z = radius * sin(theta + thetaInc) * sin(delta + deltaInc);
			sphereVer.push_back(vertexPos);
			//set up norms
			sphereNorm.push_back(XMFLOAT3(vertexPos.x / radius, vertexPos.y / radius, vertexPos.z / radius));
			//set textures
			sphereTex.push_back(XMFLOAT2(tex.x + texInc, tex.y + texInc));

			tangent.x = -radius*sinf(theta + thetaInc)*sinf(delta + deltaInc);
			tangent.y = 0.0f;
			tangent.z = radius * sinf(theta + thetaInc) *cosf(delta + deltaInc);
			sphereTangent.push_back(tangent);
			
			
			//set up new coords
			sphereIndices.push_back(sphereVer.size());
			vertexPos.x = radius * cos(theta + thetaInc)*sin(delta);
			vertexPos.y = radius * cos(delta);
			vertexPos.z = radius * sin(theta + thetaInc) * sin(delta);
			sphereVer.push_back(vertexPos);
			//set up norms
			sphereNorm.push_back(XMFLOAT3(vertexPos.x / radius, vertexPos.y / radius, vertexPos.z / radius));
			//set textures
			sphereTex.push_back(XMFLOAT2(tex.x + texInc, tex.y));

			tangent.x = -radius*sinf(theta + thetaInc)*sinf(delta);
			tangent.y = 0.0f;
			tangent.z = radius * sinf(theta + thetaInc) *cosf(delta);
			sphereTangent.push_back(tangent);

	
			sphereIndices.push_back(sphereVer.size());
			vertexPos.x = radius * cos(theta)*sin(delta);	//calculate x coord
			vertexPos.y = radius * cos(delta);	//calculate y coord			
			vertexPos.z = radius * sin(theta) * sin(delta);
			sphereVer.push_back(vertexPos);
			//set up norms
			sphereNorm.push_back(XMFLOAT3(vertexPos.x / radius, vertexPos.y / radius, vertexPos.z / radius));
			//set textures
			sphereTex.push_back(tex);

			tangent.x = -radius*sinf(theta)*sinf(delta);
			tangent.y = 0.0f;
			tangent.z = radius * sinf(theta) *cosf(delta);
			sphereTangent.push_back(tangent);

			//increase values as we move a long the x axis
			theta += thetaInc;
			tex.x += texInc;
		}

		theta = 0.0f;
		//reset angle been around 
		//increase y values
		delta += deltaInc;
		tex.y += texInc;
	}


	indexCount = vertexCount = sphereVer.size();
	VertexTypeTangent* vertices = new VertexTypeTangent[vertexCount];
	unsigned long* indices = new unsigned long[indexCount];
	for (int i = 0; i < vertexCount; i++)
	{
			vertices[i].position = sphereVer[i];
		vertices[i].normal = sphereNorm[i];
		vertices[i].texture = sphereTex[i];
		vertices[i].tangent = sphereTangent[i];
		indices[i] = sphereIndices[i];
	}

	//indices = sphereIndices.data();

	//add temp vectors to the main vector
	
	D3D11_BUFFER_DESC vertexBufferDesc = { sizeof(VertexTypeTangent) * vertexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
	vertexData = { vertices, 0 , 0 };
	device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);

	D3D11_BUFFER_DESC indexBufferDesc = { sizeof(unsigned long) * indexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_INDEX_BUFFER, 0, 0, 0 };
	indexData = { indices, 0, 0 };
	device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);



	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;
	delete[] indices;
	indices = 0;
}
