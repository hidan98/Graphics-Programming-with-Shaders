#include "CustomeLoader.h"
#include <d3dcompiler.h>

//not actualy in use
CustomeLoader::CustomeLoader()
{
}


CustomeLoader::~CustomeLoader()
{
}

void CustomeLoader::customeVertexLoader(WCHAR *name, ID3D11Device* renderer, ID3D11VertexShader* vertexShader, ID3D11InputLayout* layout)
{
	ID3D10Blob* customeVertexShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];
	ID3D10Blob* error;
	HRESULT result;

	unsigned int numberElements;

	customeVertexShaderBuffer = 0;

	D3DReadFileToBlob(name, &customeVertexShaderBuffer);

	renderer->CreateVertexShader(customeVertexShaderBuffer->GetBufferPointer(), customeVertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;

	polygonLayout[3].SemanticName = "TANGENT";
	polygonLayout[3].SemanticIndex = 0;
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlot = 0;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].InstanceDataStepRate = 0;


	numberElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	renderer->CreateInputLayout(polygonLayout, numberElements, customeVertexShaderBuffer->GetBufferPointer(), customeVertexShaderBuffer->GetBufferSize(), &layout);

	customeVertexShaderBuffer->Release();
	customeVertexShaderBuffer = nullptr;
}
