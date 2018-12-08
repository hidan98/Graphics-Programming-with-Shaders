// Colour shader.h
// Simple shader example.
#pragma once

#include "DXF.h"

using namespace std;
using namespace DirectX;

struct matrixInfo;

class SphereDepth : public BaseShader
{

public:

	SphereDepth(ID3D11Device* device, HWND hwnd);
	~SphereDepth();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &worldMatrix, const XMMATRIX &viewMatrix, const XMMATRIX &projectionMatrix);
	

private:
	void initShader(WCHAR*, WCHAR*);
	void initShader(WCHAR*, WCHAR*, WCHAR*, WCHAR*);
	void customeLoad(WCHAR*);

private:
	ID3D11Buffer* matrixBuffer;
};
