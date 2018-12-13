#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

struct matrixInfo;

class TesselatedDepthShader : public BaseShader
{

public:

	TesselatedDepthShader(ID3D11Device* device, HWND hwnd);
	~TesselatedDepthShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix, float time, float amplitude[], float angularWave[], float angularFrequency[], float phaseShift[], XMFLOAT3 camPos);


private:
	void initShader(WCHAR*, WCHAR*);
	void initShader(WCHAR* vsFilename, WCHAR* hsFilename, WCHAR* dsFilename, WCHAR* gsFilename, WCHAR* psFilename);


	struct TessellationBufferType1
	{
		XMINT4 edge;
		XMINT4 inside;
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

private:
	ID3D11Buffer* matrixBuffer;

	ID3D11Buffer* tessellationBuffer;
	ID3D11Buffer* timeBuffer;
	ID3D11Buffer* waveBuffer;
};


