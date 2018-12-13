#include "TesselatedCube.h"



TesselatedCube::TesselatedCube(ID3D11Device* device)
{
	initBuffers(device);
}


TesselatedCube::~TesselatedCube()
{
	BaseMesh::~BaseMesh();
}



void TesselatedCube::initBuffers(ID3D11Device * device)
{
	D3D11_SUBRESOURCE_DATA vertexData, indexData;

	vertexCount = 24;
	indexCount = 24;

	VertexType* vertices = new VertexType[vertexCount];
	unsigned long* indices = new unsigned long[indexCount];


	//FRONT
	// Load the vertex array with data.
	vertices[0].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);  
	vertices[0].texture = XMFLOAT2(0.0f, 0.0f);
	vertices[0].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[1].position = XMFLOAT3(-1.0f, -1.0f, -1.0f);  
	vertices[1].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[1].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[2].position = XMFLOAT3(1.0f, -1.0f, -1.0f);  
	vertices[2].texture = XMFLOAT2(1.0f, 1.0f);
	vertices[2].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);
	
	vertices[3].position = XMFLOAT3(1.0f, 1.0f, -1.0f);  
	vertices[3].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[3].normal = XMFLOAT3(0.0f, 0.0f, -1.0f);

	// Load the index array with data.
	indices[0] = 0;  
	indices[1] = 1;  
	indices[2] = 2; 
	indices[3] = 3;  

	//BACK
	vertices[4].position = XMFLOAT3(-1.0f, 1.0f, 1.0f);  
	vertices[4].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[4].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

	vertices[5].position = XMFLOAT3(-1.0f, -1.0f, 1.0f); 
	vertices[5].texture = XMFLOAT2(1.0f, 1.0f);
	vertices[5].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

	vertices[6].position = XMFLOAT3(1.0f, -1.0f, 1.0f);  
	vertices[6].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[6].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

	vertices[7].position = XMFLOAT3(1.0f, 1.0f, 1.0f);  
	vertices[7].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[7].normal = XMFLOAT3(0.0f, 0.0f, 1.0f);

	indices[4] = 7; 
	indices[5] = 6;  
	indices[6] = 5;   
	indices[7] = 4;  


	//TOP
	vertices[8].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);  
	vertices[8].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[8].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

	vertices[9].position = XMFLOAT3(1.0f, 1.0f, -1.0f);
	vertices[9].texture = XMFLOAT2(1.0f, 1.0f);
	vertices[9].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

	vertices[10].position = XMFLOAT3(1.0f, 1.0f, 1.0f);  
	vertices[10].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[10].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

	vertices[11].position = XMFLOAT3(-1.0f, 1.0f, 1.0f);  
	vertices[11].texture = XMFLOAT2(0.0f, 0.0f);
	vertices[11].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);

	// Load the index array with data.
	indices[8] = 8;  
	indices[9] = 9; 
	indices[10] = 10;  
	indices[11] = 11;  

	//BOTTOM
	vertices[12].position = XMFLOAT3(-1.0f, -1.0f, -1.0f); 
	vertices[12].texture = XMFLOAT2(0.0f, 0.0f);
	vertices[12].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	vertices[13].position = XMFLOAT3(1.0f, -1.0f, -1.0f);  
	vertices[13].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[13].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	vertices[14].position = XMFLOAT3(1.0f, -1.0f, 1.0f);  
	vertices[14].texture = XMFLOAT2(1.0f, 1.0f);
	vertices[14].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	vertices[15].position = XMFLOAT3(-1.0f,- 1.0f, 1.0f);  
	vertices[15].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[15].normal = XMFLOAT3(0.0f, -1.0f, 0.0f);

	indices[12] = 15;  
	indices[13] = 14;  
	indices[14] = 13;  
	indices[15] = 12;  

	//LEFT
	vertices[16].position = XMFLOAT3(-1.0f, -1.0f, -1.0f); 
	vertices[16].texture = XMFLOAT2(0.0f, 0.0f);
	vertices[16].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

	vertices[17].position = XMFLOAT3(-1.0f, 1.0f, -1.0f);  
	vertices[17].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[17].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

	vertices[18].position = XMFLOAT3(-1.0f, 1.0f, 1.0f); 
	vertices[18].texture = XMFLOAT2(1.0f, 1.0f);
	vertices[18].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

	vertices[19].position = XMFLOAT3(-1.0f, -1.0f, 1.0f);  
	vertices[19].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[19].normal = XMFLOAT3(-1.0f, 0.0f, 0.0f);

	indices[16] = 16;  
	indices[17] = 17;  
	indices[18] = 18;  
	indices[19] = 19;  

	//RIGHT
	vertices[20].position = XMFLOAT3(1.0f, -1.0f, -1.0f);
	vertices[20].texture = XMFLOAT2(0.0f, 0.0f);
	vertices[20].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

	vertices[21].position = XMFLOAT3(1.0f, 1.0f, -1.0f);
	vertices[21].texture = XMFLOAT2(0.0f, 1.0f);
	vertices[21].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

	vertices[22].position = XMFLOAT3(1.0f, 1.0f, 1.0f);
	vertices[22].texture = XMFLOAT2(1.0f, 1.0f);
	vertices[22].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

	vertices[23].position = XMFLOAT3(1.0f, -1.0f, 1.0f);
	vertices[23].texture = XMFLOAT2(1.0f, 0.0f);
	vertices[23].normal = XMFLOAT3(1.0f, 0.0f, 0.0f);

	indices[20] = 23;  
	indices[21] = 22;  
	indices[22] = 21;   
	indices[23] = 20;  

	D3D11_BUFFER_DESC vertexBufferDesc = { sizeof(VertexType) * vertexCount, D3D11_USAGE_DEFAULT, D3D11_BIND_VERTEX_BUFFER, 0, 0, 0 };
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


void TesselatedCube::sendData(ID3D11DeviceContext * deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_4_CONTROL_POINT_PATCHLIST);
}