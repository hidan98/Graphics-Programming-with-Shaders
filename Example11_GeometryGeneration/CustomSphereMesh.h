
#pragma once
#include "D3D.h"
#include "BaseMesh.h"




class CustomSphereMesh : public BaseMesh
{
public:
	CustomSphereMesh(ID3D11Device* device);
	~CustomSphereMesh();

	void sendData(ID3D11DeviceContext* deviceContext);

protected:
	void initBuffers(ID3D11Device* device);
	float theta;
	float delta;
	float thetaInc;
	float deltaInc;
	XMFLOAT2 tex;
	XMFLOAT3 vertexPos;
	XMFLOAT3 norm;
	XMFLOAT3 tangent;
	float X;
	float Y;

	int seg = 50;
	float radius = 1.f;

	
};

