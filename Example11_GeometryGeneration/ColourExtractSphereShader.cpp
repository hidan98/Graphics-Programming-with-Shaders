#include "ColourExtractSphereShader.h"
#include <d3dcompiler.h>

ColourExtractSphereShader::ColourExtractSphereShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	loader = new CustomeLoader;
	initShader(L"sphere_vs.cso", L"sphere_hs.cso", L"sphere_ds.cso", L"SphereBrightExtrac_ps.cso");

}


ColourExtractSphereShader::~ColourExtractSphereShader()
{
	// Release the sampler state.
	if (sampleState)
	{
		sampleState->Release();
		sampleState = 0;
	}

	// Release the matrix constant buffer.
	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = 0;
	}

	// Release the layout.
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
	if (brightBuffer)
	{
		brightBuffer->Release();
		brightBuffer = 0;
	}

	delete loader;
	loader = NULL;
	//Release base shader components
	BaseShader::~BaseShader();
}

void ColourExtractSphereShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC tessellationBufferDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC brigthBufferDesc;


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

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&matrixBufferDesc, NULL, &matrixBuffer);

	// Create a texture sampler state description.
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	// Create the texture sampler state.
	renderer->CreateSamplerState(&samplerDesc, &sampleState);



	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = 0;
	lightBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&lightBufferDesc, NULL, &lightBuffer);

	brigthBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	brigthBufferDesc.ByteWidth = sizeof(BrightnessBufferType);
	brigthBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	brigthBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	brigthBufferDesc.MiscFlags = 0;
	brigthBufferDesc.StructureByteStride = 0;
	renderer->CreateBuffer(&brigthBufferDesc, NULL, &brightBuffer);

}
void ColourExtractSphereShader::customeLoad(WCHAR* filename)
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

void ColourExtractSphereShader::initShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* psFilename)
{
	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	initShader(vsFilename, psFilename);

	// Load other required shaders.
	loadHullShader(hsFilename);
	loadDomainShader(dsFilename);
}

void ColourExtractSphereShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normal, Light* light[2], float brightness)
{
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;
	XMMATRIX tworld, tview, tproj;


	// Transpose the matrices to prepare them for the shader.
	tworld = XMMatrixTranspose(worldMatrix);
	tview = XMMatrixTranspose(viewMatrix);
	tproj = XMMatrixTranspose(projectionMatrix);

	// Sned matrix data
	result = deviceContext->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	dataPtr = (MatrixBufferType*)mappedResource.pData;
	dataPtr->world = tworld;// worldMatrix;
	dataPtr->view = tview;
	dataPtr->projection = tproj;
	deviceContext->Unmap(matrixBuffer, 0);
	deviceContext->DSSetConstantBuffers(0, 1, &matrixBuffer);

	//send light info
	deviceContext->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	LightBufferType* ligthPtr = (LightBufferType*)mappedResource.pData;
	for (int i = 0; i < 2; i++)
	{
		ligthPtr->ambient[i] = light[i]->getAmbientColour();
		ligthPtr->diffuse[i] = light[i]->getDiffuseColour();
		ligthPtr->direction[i].x = light[i]->getDirection().x;
		ligthPtr->direction[i].y = light[i]->getDirection().y;
		ligthPtr->direction[i].z = light[i]->getDirection().z;
		ligthPtr->direction[i].w = 1.0f;
	}
	deviceContext->Unmap(lightBuffer, 0);
	deviceContext->PSSetConstantBuffers(0, 1, &lightBuffer);

	// Set shader texture and sampler resource in the pixel shader.
	deviceContext->PSSetShaderResources(0, 1, &texture);
	deviceContext->PSSetShaderResources(1, 1, &normal);
	deviceContext->PSSetSamplers(0, 1, &sampleState);

	//send brightness info
	deviceContext->Map(brightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	BrightnessBufferType* brightPtr = (BrightnessBufferType*)mappedResource.pData;
	brightPtr->bright = brightness;
	brightPtr->padding = XMFLOAT3(0.0f, 0.0f, 0.0f);
	deviceContext->Unmap(lightBuffer, 0);
	deviceContext->PSSetConstantBuffers(1, 1, &brightBuffer);
}




