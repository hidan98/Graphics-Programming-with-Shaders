// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{

}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{



	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// Create Mesh object and shader object
	mesh = new PointMesh(renderer->getDevice(), renderer->getDeviceContext());
	cube = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	plane = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	tesselatedCube = new TesselatedCube(renderer->getDevice());

	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);
	tessDepth = new TesselatedDepthShader(renderer->getDevice(), hwnd);
	textureMgr->loadTexture("brick", L"res/brick1.dds");
	textureMgr->loadTexture("bunny", L"res/bunny.png");
	textureMgr->loadTexture("normal", L"res/NormalMap.png");
	geometryShader = new GeometryShader(renderer->getDevice(), hwnd);
	tessShadow = new TesselatedShadowShader(renderer->getDevice(), hwnd);

	hightShader = new HightMapSphereShader(renderer->getDevice(), hwnd);

	int shadowmapWidth = 2048 * 2;
	int shadowmapHeight = 2048 *2;
	int sceneWidth = 100;
	int sceneHeight = 100;

	
	shadowInfo_ = new shadowInfo;
	light[0] = new Light;
	light[0]->setAmbientColour(0.3f, 0.3f, 0.3f, 1.0f);
	light[0]->setDiffuseColour(1.0f, 0.0f, 0.0f, 1.0f);
	light[0]->setDirection(0.0f, -0.2f, 0.7f);
	light[0]->setPosition(0.f, 0.f, -10.f);
	light[0]->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 100.f);
	//shadowInfo_->light[0]->generateViewMatrix();

	light[1] = new Light;
	light[1]->setAmbientColour(0.30f, 0.30f, 0.30f, 1.0f);
	light[1]->setDiffuseColour(0.0f, 0.0f, 1.0f, 1.0f);
	light[1]->setDirection(0.0f, -1.f, 0.01f);
	light[1]->setPosition(0.f, 10.f, -10.f);
	light[1]->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 100.f);
	//shadowInfo_->light[1]->generateViewMatrix();

	shadowInfo_->shadowMap[0] = new RenderTexture(renderer->getDevice(), shadowmapWidth, shadowmapHeight, 0.1f, 100.f);
	shadowInfo_->shadowMap[1] = new RenderTexture(renderer->getDevice(), shadowmapWidth, shadowmapHeight, 0.1f, 100.f);

	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth / 4, screenHeight / 4, screenWidth / -4, screenHeight / -4);

	orthoMesh1 = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth / 4, screenHeight / 4, screenWidth / 4, screenHeight / 4);

	stopTime = true;

	
	sphere = new CustomSphereMesh(renderer->getDevice());
}

void App1::depthPass()
{
	if (!stopTime)
	{
		time += timer->getTime();
	}
	
	matrixInfo matrixInfo_;


	for (int i = 0; i < 2; i++)
	{

		shadowInfo_->shadowMap[i]->setRenderTarget(renderer->getDeviceContext());
		shadowInfo_->shadowMap[i]->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);
		


		// get the world, view, and projection matrices from the camera and d3d objects.
		
		light[i]->generateViewMatrix();
		
		XMMATRIX lightViewMatrix = light[i]->getViewMatrix();
		XMMATRIX lightProjectionMatrix = light[i]->getOrthoMatrix();
		XMMATRIX worldMatrix = renderer->getWorldMatrix();


		// get the world, view, and projection matrices from the camera and d3d objects.

		worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);
	
		// Render floor
		plane->sendData(renderer->getDeviceContext());
		depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
		depthShader->render(renderer->getDeviceContext(), plane->getIndexCount());



		worldMatrix = renderer->getWorldMatrix();
		worldMatrix = XMMatrixTranslation(0.f, 7.f, 5.f);
		//XMMATRIX scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
		//matrixInfo_.worldMatrix = XMMatrixMultiply(matrixInfo_.worldMatrix, scaleMatrix);


		// Render model
		tesselatedCube->sendData(renderer->getDeviceContext());
		tessDepth->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, time, amplitude, angularWave, angularFrequency, phaseShift);
		tessDepth->render(renderer->getDeviceContext(), tesselatedCube->getIndexCount());

		// Set back buffer as render target and reset view port.
		renderer->setBackBufferRenderTarget();
		renderer->resetViewport();

	}

	
}

void App1::shadowPass()
{

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);
	camera->update();

	// get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();

	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);
	// Render floor
	plane->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,	textureMgr->getTexture("brick"), light, shadowInfo_->shadowMap[0]->getShaderResourceView(), shadowInfo_->shadowMap[1]->getShaderResourceView());
	shadowShader->render(renderer->getDeviceContext(), plane->getIndexCount());

	// Render model
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(0.f, 7.f, 5.f);
//	XMMATRIX scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);
	//worldMatrix = XMMatrixMultiply(worldMatrix, scaleMatrix);
	tesselatedCube->sendData(renderer->getDeviceContext());
	tessShadow->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("bunny"),  shadowInfo_->shadowMap[0]->getShaderResourceView(), shadowInfo_->shadowMap[1]->getShaderResourceView(), light, time, amplitude, angularWave, angularFrequency, phaseShift);
	tessShadow->render(renderer->getDeviceContext(), tesselatedCube->getIndexCount());





	////hight map sphere 
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(0.0f, 10.0f, 10.0f);
	sphere->sendData(renderer->getDeviceContext());
	hightShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("bunny"), textureMgr->getTexture("normal"), light[0]);
	hightShader->render(renderer->getDeviceContext(), sphere->getIndexCount());


	//worldMatrix = renderer->getWorldMatrix();
	//XMMATRIX orthView = camera->getOrthoViewMatrix();
	//XMMATRIX orthProj = renderer->getOrthoMatrix();
	//renderer->setZBuffer(false);
	//orthoMesh->sendData(renderer->getDeviceContext());
	//textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthView, orthProj, shadowInfo_->shadowMap[0]->getShaderResourceView());
	//textureShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	//renderer->setZBuffer(true);


	//worldMatrix = renderer->getWorldMatrix();
	//renderer->setZBuffer(false);
	//orthoMesh1->sendData(renderer->getDeviceContext());
	//textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthView, orthProj, shadowInfo_->shadowMap[1]->getShaderResourceView());
	//textureShader->render(renderer->getDeviceContext(), orthoMesh1->getIndexCount());
	//renderer->setZBuffer(true);


	gui();
	renderer->endScene();
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.

}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}
	
	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	depthPass();
	shadowPass();

	return true;
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);

	ImGui::SliderFloat("amplitude[0]", &amplitude[0], 0, 5.0f);
	ImGui::SliderFloat("amplitude[1]", &amplitude[1], 0, 5.0f);
	ImGui::SliderFloat("angularWave[0]", &angularWave[0], 0, 20);
	ImGui::SliderFloat("angularWave[1]", &angularWave[1], 0, 20);

	ImGui::SliderFloat("angularFrequency[0]", &angularFrequency[0], 0, 20);
	ImGui::SliderFloat("angularFrequency[1]", &angularFrequency[1], 0, 20);

	ImGui::SliderFloat("phaseShift[0]", &phaseShift[0], 0, 5);
	ImGui::SliderFloat("phaseShift[1]", &phaseShift[1], 0, 5);

	ImGui::Checkbox("Stop time", &stopTime);

	// Render UI
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

