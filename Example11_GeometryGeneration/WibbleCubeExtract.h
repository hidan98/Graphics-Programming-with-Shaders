#pragma once
#include "DXF.h"

using namespace std;
using namespace DirectX;


class WibbleCubeExtract : public BaseShader
{

public:

	WibbleCubeExtract(ID3D11Device* device, HWND hwnd);
	~WibbleCubeExtract();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Light* light[2], float bright);


private:
	void initShader(WCHAR* vsFilename, WCHAR* psFilename);
	void initShader(WCHAR* vsFilename, WCHAR* dsFilename, WCHAR* hsFilename, WCHAR* gsFilename, WCHAR* psFilename);

	struct LightBufferType
	{
		XMFLOAT4 ambient[2];
		XMFLOAT4 diffuse[2];
		XMFLOAT4 direction[2];
	};
	struct BrightnessBufferType
	{
		float bright;
		XMFLOAT3 padding;
	};

private:
	ID3D11Buffer * matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* brightBuffer;
	ID3D11Buffer* lightBuffer;
};
