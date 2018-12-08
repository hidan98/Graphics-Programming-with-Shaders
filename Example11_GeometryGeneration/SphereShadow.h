#pragma once
#include "DXF.h"

using namespace std;
using namespace DirectX;

struct shadowInfo;


class SphereShadow : public BaseShader
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

public:

	SphereShadow(ID3D11Device* device, HWND hwnd);
	~SphereShadow();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Light *info_[2], ID3D11ShaderResourceView* depth, ID3D11ShaderResourceView* depth1, ID3D11ShaderResourceView* normal);

private:
	void initShader(WCHAR*, WCHAR*);
	void initShader(WCHAR*, WCHAR*, WCHAR*, WCHAR*);

	void customeLoad(WCHAR*);

private:
	ID3D11Buffer* matrixBuffer;
	ID3D11SamplerState* sampleState;
	ID3D11SamplerState* sampleStateShadow;
	ID3D11Buffer* lightBuffer;

	ID3D11Buffer* lightMatrixBuffer;
};
