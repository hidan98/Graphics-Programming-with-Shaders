#pragma once
#include "D3D.h"
#include "BaseMesh.h"
class TesselatedCube : public BaseMesh
{
public:
	TesselatedCube(ID3D11Device* device);
	~TesselatedCube();

	void sendData(ID3D11DeviceContext* deviceContext);

protected:
	void initBuffers(ID3D11Device* device);
};

