// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "GeometryShader.h"
#include "DepthShader.h"
#include "ShadowShader.h"
#include "TextureShader.h"
#include "TesselatedCube.h"
#include "TesselatedDepthShader.h"
#include "TesselatedShadowShader.h"
#include "HightMapSphereShader.h"
#include "CustomSphereMesh.h"

struct matrixInfo
{
	XMMATRIX lightViewMatrix;
	XMMATRIX lightProjectionMatrix;
	XMMATRIX worldMatrix;
};

struct shadowInfo
{
	Light* light[2];
	RenderTexture* shadowMap[2];

};


class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);
	void depthPass();
	void shadowPass();
	bool frame();

protected:
	bool render();
	void gui();

	

private:
	TextureShader* textureShader;
	GeometryShader* geometryShader;
	DepthShader* depthShader;
	ShadowShader* shadowShader;
	
	TesselatedDepthShader* tessDepth;
	TesselatedShadowShader* tessShadow;
	HightMapSphereShader* hightShader;

	CubeMesh* cube;
	PlaneMesh* plane;
	PointMesh* mesh;

	
	//Light* light1;
	TesselatedCube* tesselatedCube;

	OrthoMesh* orthoMesh;
	OrthoMesh* orthoMesh1;

	RenderTexture* shadow;
	RenderTexture* shadow1;

	float amplitude[2] = { 0.8f, 0.2f };
	float angularWave[2] = { 1.0f, 8.0f };
	float angularFrequency[2] = { 1.0f, 8.0f };

	float phaseShift[2] = { 0.0f, 1.0f };

	float time;
	bool stopTime;

	CustomSphereMesh* sphere;

	shadowInfo* shadowInfo_;
	
	Light* light1;
};

#endif