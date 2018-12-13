#pragma once
#include "DXF.h"


using namespace std;
using namespace DirectX;

struct shadowInfo;

class TesselatedShadowShader : public BaseShader
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct lightMatrixType
	{
		XMMATRIX lightView[2];
		XMMATRIX lightProjection[2];
	};

	struct LightBufferType
	{
		XMFLOAT4 ambient[2];
		XMFLOAT4 diffuse[2];
		XMFLOAT4 direction[2];
	};

	struct WaveBufferType
	{		
		XMFLOAT4 time;
		XMFLOAT2 height;
		XMFLOAT2 waveLenght;
		XMFLOAT2 frequency;
		XMFLOAT2 shift;
	};

	struct timeBufferType
	{
		float time;
		XMFLOAT3 padding;
	};
	struct cameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

public:

	TesselatedShadowShader(ID3D11Device* device, HWND hwnd);
	~TesselatedShadowShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView*depthMap, ID3D11ShaderResourceView* depthMap1, Light* info_[2], float time, float amplitude[], float angularWave[], float angularFrequency[], float phaseShift[], XMFLOAT3 camPos);


private:
	void initShader(WCHAR*, WCHAR*);
	void initShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* gsFilename, WCHAR* psFilename);
	

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11Buffer* lightBuffer;

	ID3D11Buffer* tessellationBuffer;
	ID3D11Buffer* waveBuffer;
	ID3D11Buffer* timeBuffer1;
	ID3D11Buffer* camBuffer;

	ID3D11Buffer* lightMatrixBuffer;
};
