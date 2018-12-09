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
#include "HoriszontalBlurShader.h"
#include "VerticalBlurShader.h"
#include "ExtractLightShader.h"
#include "CombineShader.h"
#include "SphereShadow.h"
#include "SphereDepth.h"
#include "ColourExtractSphereShader.h"
#include "WibbleCubeExtract.h"

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
	
	void extractLight();
	void depthPass();
	void shadowPass();
	void horizontalBlur();
	void verticalBlur();
	void bloomPass();
	void combinePass();
	bool frame();

	void finalPass();


	void renderNormal();

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
	SphereShadow* sphereShadow;
	SphereDepth* sphereDepth;
	ColourExtractSphereShader* sphereExtract;
	WibbleCubeExtract* wibbleExtract;

	CubeMesh* cube;
	PlaneMesh* plane;
	PointMesh* mesh;
	CubeMesh* anotherCube;

	
	//Light* light1;
	TesselatedCube* tesselatedCube;

	OrthoMesh* orthoMesh;
	OrthoMesh* orthoMesh1;
	OrthoMesh* screenOrtho;
	RenderTexture* shadow;
	RenderTexture* shadow1;

	RenderTexture* horizontalBlurTexture;
	RenderTexture* verticalBlurTexture;
	RenderTexture* bloomTexture;
	RenderTexture* texture;
	RenderTexture* combineTexture;
	RenderTexture* testTexture;


	HoriszontalBlurShader* horizontalShader;
	VerticalBlurShader* verticalShader;
	ExtractLightShader* extractShader;
	CombineShader* combine;

	float amplitude[2] = { 0.8f, 0.2f };
	float angularWave[2] = { 1.0f, 8.0f };
	float angularFrequency[2] = { 1.0f, 8.0f };

	float phaseShift[2] = { 0.0f, 1.0f };

	float time;
	bool stopTime;

	CustomSphereMesh* sphere;

	shadowInfo* shadowInfo_;
	
	Light* light[2];

	float colour[4];
	float colour1[4];
	float colourAmbiant[4];
	float colourAmbiant1[4];
	OrthoMesh* orthoMeshBlur;

	float brightThreshHold;
	int times;
};

#endif