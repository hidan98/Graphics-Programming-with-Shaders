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

	struct TimeBufferType
	{
		XMFLOAT4 time;
		XMFLOAT2 amplitude;
		XMFLOAT2 angularWave;
		XMFLOAT2 angularFrequency;
		XMFLOAT2 phaseShift;
		XMFLOAT4 padding;
	};

	struct timeBuffer1Type
	{
		float time;
		XMFLOAT3 padding;
	};

public:

	TesselatedShadowShader(ID3D11Device* device, HWND hwnd);
	~TesselatedShadowShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView*depthMap, ID3D11ShaderResourceView* depthMap1, shadowInfo* info_, float time, float amplitude[], float angularWave[], float angularFrequency[], float phaseShift[]);


private:
	void initShader(WCHAR*, WCHAR*);
	void initShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* gsFilename, WCHAR* psFilename);
	

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11Buffer* lightBuffer;

	ID3D11Buffer* tessellationBuffer;
	ID3D11Buffer* timeBuffer;
	ID3D11Buffer* timeBuffer1;


	ID3D11Buffer* lightMatrixBuffer;
};
