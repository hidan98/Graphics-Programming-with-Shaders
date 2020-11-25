#include "SphereShadow.h"
#include <d3dcompiler.h>

SphereShadow::SphereShadow(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	loader = new CustomeLoader;
	initShader(L"sphere_vs.cso", L"ShadowSphere_ds.cso", L"sphere_hs.cso", L"sphere_ps.cso");
	
}


SphereShadow::~SphereShadow()
{
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}
	if (layout)
	{
		layout->Release();
		layout = 0;
	}
	if (lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = 0;
	}

	delete loader;
	loader = NULL;
	//Release base shader components
	BaseShader::~BaseShader();
}


void SphereShadow::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC lightMatrixDesc;

	// Load (+ compile) shader files
	//loader->customeVertexLoader(vsFilename, renderer, vertexShader, layout);
	customeLoad(vsFilename);
	loadPixelShader(psFilename);

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	lightMatrixDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightMatrixDesc.ByteWidth = sizeof(lightMatrixType);
	lightMatrixDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightMatrixDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightMatrixDesc.MiscFlags = 0;
	lightMatrixDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&lightMatrixDesc, NULL, &lightMatrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_MIRROR;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	renderer->CreateSamplerState(&samplerDesc, &sampleState);

	// Sampler for shadow map sampling.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDesc.BorderColor[0] = 1.0f;
	samplerDesc.BorderColor[1] = 1.0f;
	samplerDesc.BorderColor[2] = 1.0f;
	samplerDesc.BorderColor[3] = 1.0f;
	renderer->CreateSamplerState(&samplerDesc, &sampleStateShadow);

	// Setup light buffer
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);

}

void SphereShadow::initShader(WCHAR* vsFilename, WCHAR* dsFilename , WCHAR* hsFilename, WCHAR* psFilename)
{
	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	initShader(vsFilename, psFilename);

	// Load other required shaders.
	loadHullShader(hsFilename);
	loadDomainShader(dsFilename);
}


void SphereShadow::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, Light *info_[2], ID3D11ShaderResourceView* depth, ID3D11ShaderResourceView* depth1,ID3D11ShaderResourceView* normal)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	LightBufferType* lightPtr;
	lightMatrixType* lightMatrixPtr;

	// Transpose the matrices to prepare them for the shader.
	XMMATRIX tworld = XMMatrixTranspose(worldMatrix);
	XMMATRIX tview = XMMatrixTranspose(viewMatrix);
	XMMATRIX tproj = XMMatrixTranspose(projectionMatrix);

	// Lock the constant buffer so it can be written to.
	deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->VSSetConstantBuffers(0, 1, &matrixBuffer);


	//Additional
	// Send light data to pixel shader
	deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightPtr = (LightBufferType*)mappedResource.pData;
	for (int i = 0; i < 2; i++)
	{
		lightPtr->ambient[i] = info_[i]->getAmbientColour();
		lightPtr->diffuse[i] = info_[i]->getDiffuseColour();
		lightPtr->direction[i].x = info_[i]->getDirection().x;
		lightPtr->direction[i].y = info_[i]->getDirection().y;
		lightPtr->direction[i].z = info_[i]->getDirection().z;
		lightPtr->direction[i].w = 1.0f;
	}

	deviceContext->Unmap(lightBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &lightBuffer);

	// Set shader texture resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetShaderResources(1, 1, &normal);
	deviceContext->PSSetShaderResources(2, 1, &depth1);
	deviceContext->PSSetShaderResources(3, 1, &depth);
	deviceContext->PSSetSamplers(0, 1, &sampleState);
	deviceContext->PSSetSamplers(1, 1, &sampleStateShadow);



	deviceContext->Map(lightMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightMatrixPtr = (lightMatrixType*)mappedResource.pData;
	for (int i = 0; i < 2; i++)
	{
		lightMatrixPtr->lightView[i] = XMMatrixTranspose(info_[i]->getViewMatrix());
		lightMatrixPtr->lightProjection[i] = XMMatrixTranspose(info_[i]->getOrthoMatrix());
	}
	deviceContext->Unmap(lightMatrixBuffer, 0);
	deviceContext->DSSetConstantBuffers(1, 1, &lightMatrixBuffer);
}


void SphereShadow::customeLoad(WCHAR* filename)
{
	ID3D10Blob* customeVertexShaderBuffer;
	D3D11_INPUT_ELEMENT_DESC polygonLayout[4];

	unsigned int numberElements;

	customeVertexShaderBuffer = 0;

	//read file in
	D3DReadFileToBlob(filename, &customeVertexShaderBuffer);
	//set up new vertex buffer
	renderer->CreateVertexShader(customeVertexShaderBuffer->GetBufferPointer(), customeVertexShaderBuffer->GetBufferSize(), NULL, &vertexShader);

	//create input layout - data will be put in to the buffer in this order (position, texcoord, normal and tangent)
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

	//number of elements in the buffer
	numberElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

	//create input layout from the polygone layout
	renderer->CreateInputLayout(polygonLayout, numberElements, customeVertexShaderBuffer->GetBufferPointer(), customeVertexShaderBuffer->GetBufferSize(), &layout);

	//relese buffer - no longer needed
	customeVertexShaderBuffer->Release();
	customeVertexShaderBuffer = nullptr;

}
