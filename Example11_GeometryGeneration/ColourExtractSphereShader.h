#pragma once
#include "DXF.h"

using namespace std;
using namespace DirectX;


class ColourExtractSphereShader : public BaseShader
{

public:

	ColourExtractSphereShader(ID3D11Device* device, HWND hwnd);
	~ColourExtractSphereShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* normal, Light* light[2]);


private:
	void initShader(WCHAR* vsFilename, WCHAR* psFilename);
	void initShader(WCHAR* vsFilename, WCHAR* dsFilename, WCHAR* hsFilename, WCHAR* psFilename);
	void customeLoad(WCHAR*);

	struct LightBufferType
	{
		XMFLOAT4 ambient[2];
		XMFLOAT4 diffuse[2];
		XMFLOAT4 direction[2];
	};

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;

	ID3D11Buffer* lightBuffer;
};
