#pragma once
#include"DXF.h"

using namespace std;
using namespace DirectX;


class CombineShader : public BaseShader
{
private:



public:
	CombineShader(ID3D11Device* device, HWND hwnd);
	~CombineShader();

	void setShderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, ID3D11ShaderResourceView* texture1);

private:
	void initShader(WCHAR*, WCHAR*);

	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11Buffer* screenSizeBuffer;
};

