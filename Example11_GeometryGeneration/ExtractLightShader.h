#include "DXF.h"

using namespace std;
using namespace DirectX;


class ExtractLightShader : public BaseShader
{

public:

	ExtractLightShader(ID3D11Device* device, HWND hwnd);
	~ExtractLightShader();

	void setShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX &world, const XMMATRIX &view, const XMMATRIX &projection, ID3D11ShaderResourceView* texture, Light* light[2]);


private:
	void initShader(WCHAR* vsFilename, WCHAR* psFilename);
	void initShader(WCHAR* vsFilename, WCHAR* dsFilename, WCHAR* hsFilename, WCHAR* psFilename);


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
