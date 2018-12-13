#pragma once
#include "DXF.h"

class CustomeLoader 
{
public:
	CustomeLoader();
	~CustomeLoader();
	void customeVertexLoader(WCHAR*, ID3D11Device* rederer, ID3D11VertexShader* vertexShader, ID3D11InputLayout* layout);

private:
	
};

