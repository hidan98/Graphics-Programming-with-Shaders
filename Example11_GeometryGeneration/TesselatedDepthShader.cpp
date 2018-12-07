// depth shader.cpp
#include "TesselatedDepthShader.h"
#include "App1.h"

TesselatedDepthShader::TesselatedDepthShader(ID3D11Device* device, HWND hwnd) : BaseShader(device, hwnd)
{
	initShader(L"tessellation_vs.cso", L"tessellation_hs.cso", L"tessellation_ds.cso", L"depth_gs.cso", L"depth_ps.cso");
}


TesselatedDepthShader::~TesselatedDepthShader()
{
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

	//Release base shader components
	BaseShader::~BaseShader();
}


void TesselatedDepthShader::initShader(WCHAR* vsFilename, WCHAR* psFilename)
{
	D3D11_BUFFER_DESC matrixBufferDesc;
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

	// Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
	TessellationBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	TessellationBufferDesc.ByteWidth = sizeof(TessellationBufferType1);
	TessellationBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	TessellationBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	TessellationBufferDesc.MiscFlags = 0;
	TessellationBufferDesc.StructureByteStride = 0;

	// Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
	renderer->CreateBuffer(&TessellationBufferDesc, NULL, &tessellationBuffer);



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

void TesselatedDepthShader::initShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* gsFilename ,WCHAR* psFilename)
{
	// InitShader must be overwritten and it will load both vertex and pixel shaders + setup buffers
	initShader(vsFilename, psFilename);

	// Load other required shaders.
	loadHullShader(hsFilename);
	loadDomainShader(dsFilename);
	loadGeometryShader(gsFilename);
}


void TesselatedDepthShader::setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, float time, float amplitude[], float angularWave[], float angularFrequency[], float phaseShift[])
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	MatrixBufferType* dataPtr;

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

	deviceContext->Unmap(timeBuffer, 0);
	deviceContext->DSSetConstantBuffers(1, 1, &timeBuffer);
	deviceContext->DSSetConstantBuffers(0, 1, &matrixBuffer);

	deviceContext->Map(timeBuffer1, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	timeBuffer1Type* dataPtr99 = (timeBuffer1Type*)mappedResource.pData;
	dataPtr99->time = time;
	dataPtr99->padding = XMFLOAT3(0.0f, 0.0f, 0.0f);
	deviceContext->Unmap(timeBuffer1, 0);
	deviceContext->GSSetConstantBuffers(1, 1, &timeBuffer1);
}



