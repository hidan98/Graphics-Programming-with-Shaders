#include "TesselatedShadowShader.h"
#include "App1.h"

TesselatedShadowShader::TesselatedShadowShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"tessellation_vs.cso", L"tessellation_hs.cso", L"tessellation_ds.cso", L"shadow_gs.cso", L"shadow_ps.cso");
}


TesselatedShadowShader::~TesselatedShadowShader()
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

	//Release base shader components
	BaseShader::~BaseShader();
}


void TesselatedShadowShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
	D3D11_SAMPLER_DESC samplerDesc;
	D3D11_BUFFER_DESC lightBufferDesc;
	D3D11_BUFFER_DESC lightMatrixDesc;

	D3D11_BUFFER_DESC TessellationBufferDesc;
	D3D11_BUFFER_DESC TimeBufferDesc;
	D3D11_BUFFER_DESC TimeBuffer1Desc;

	// Load (+ compile) shader files
	loadVertexShader(vsFilename);
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






	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	TimeBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	TimeBufferDesc.ByteWidth = sizeof(TimeBufferType);
	TimeBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	TimeBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TimeBufferDesc.MiscFlags = 0;
	TimeBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&TimeBufferDesc, NULL, &timeBuffer);


	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	TimeBuffer1Desc.Usage = D3D11_USAGE_DYNAMIC;
	TimeBuffer1Desc.ByteWidth = sizeof(TimeBufferType);
	TimeBuffer1Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	TimeBuffer1Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TimeBuffer1Desc.MiscFlags = 0;
	TimeBuffer1Desc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&TimeBuffer1Desc, NULL, &timeBuffer1);

}

void TesselatedShadowShader::initShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* gsFilename, WCHAR* psFilename)
{
	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	initShader(vsFilename, psFilename);

	// Load other required shaders.
	loadHullShader(hsFilename);
	loadDomainShader(dsFilename);
	loadGeometryShader(gsFilename);
}

void TesselatedShadowShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView*depthMap, ID3D11ShaderResourceView* depthMap1, Light* info_[2], float time, float amplitude[], float angularWave[], float angularFrequency[], float phaseShift[])
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
	deviceContext->GSSetConstantBuffers(0, 1, &matrixBuffer);
	deviceContext->DSSetConstantBuffers(0, 1, &matrixBuffer);
	
	

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
	deviceContext->PSSetShaderResources(1, 1, &depthMap);
	deviceContext->PSSetShaderResources(2, 1, &depthMap1);
	deviceContext->PSSetSamplers(0, 1, &sampleState);
	deviceContext->PSSetSamplers(1, 1, &sampleStateShadow);


	// Lock the constant buffer so it can be written to.
	deviceContext->Map(timeBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	TimeBufferType* dataPtr4 = (TimeBufferType*)mappedResource.pData;
	dataPtr4->time.x = time;
	dataPtr4->time.y = time;
	dataPtr4->time.z = time;
	dataPtr4->time.w = time;

	dataPtr4->amplitude.x = amplitude[0];
	dataPtr4->angularFrequency.x = angularFrequency[0];
	dataPtr4->angularWave.x = angularWave[0];
	dataPtr4->phaseShift.x = phaseShift[0];

	dataPtr4->amplitude.y = amplitude[1];
	dataPtr4->angularFrequency.y = angularFrequency[1];
	dataPtr4->angularWave.y = angularWave[1];
	dataPtr4->phaseShift.y = phaseShift[1];

	dataPtr4->padding = XMFLOAT4(0.0f, 0, 0, 0);

	deviceContext->Unmap(timeBuffer, 0);
	deviceContext->DSSetConstantBuffers(1, 1, &timeBuffer);



	// Set shader texture resource in the pixel shader.


	deviceContext->GSSetConstantBuffers(0, 1, &matrixBuffer);

	deviceContext->Map(timeBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	timeBuffer1Type* dataPtr99 = (timeBuffer1Type*)mappedResource.pData;
	dataPtr99->time = time;
	dataPtr99->padding = XMFLOAT3(0.0f, 0.0f, 0.0f);
	deviceContext->Unmap(timeBuffer1, 0);
	deviceContext->GSSetConstantBuffers(2, 1, &timeBuffer1);


	deviceContext->Map(lightMatrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	lightMatrixPtr = (lightMatrixType*)mappedResource.pData;
	for (int i = 0; i < 2; i++)
	{
		lightMatrixPtr->lightView[i] = XMMatrixTranspose(info_[i]->getViewMatrix());
		lightMatrixPtr->lightProjection[i] = XMMatrixTranspose(info_[i]->getOrthoMatrix());
	}
	deviceContext->Unmap(lightMatrixBuffer, 0);
	deviceContext->GSSetConstantBuffers(2, 1, &lightMatrixBuffer);
}

