#pragma once
#include"DXF.h"

using namespace std;
using namespace DirectX;


class HoriszontalBlurShader : public BaseShader
{
private:

	struct screenSizeBufferType
	{
		float screenWidth;
		int NumBlurs;
		XMFLOAT2 padding;
	};

public:
	HoriszontalBlurShader(ID3D11Device* device, HWND hwnd);
	~HoriszontalBlurShader();

	void setShderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, float width, int times);

private:
	void initShader(WCHAR*, WCHAR*);

	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* screenSizeBuffer;
};

