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

	// Create Mesh objects
	mesh = new PointMesh(renderer->getDevice(), renderer->getDeviceContext());
	cube = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	plane = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	tesselatedCube = new TesselatedCube(renderer->getDevice());
	sphere = new CustomSphereMesh(renderer->getDevice());

	//load shaders
	textureShader = new TextureShader(renderer->getDevice(), hwnd);
	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);
	tessDepth = new TesselatedDepthShader(renderer->getDevice(), hwnd);
	tessShadow = new TesselatedShadowShader(renderer->getDevice(), hwnd);
	combine = new CombineShader(renderer->getDevice(), hwnd);
	bumpShader = new BumpMapSphere(renderer->getDevice(), hwnd);
	sphereShadow = new SphereShadow(renderer->getDevice(), hwnd);
	sphereDepth = new SphereDepth(renderer->getDevice(), hwnd);
	sphereExtract = new ColourExtractSphereShader(renderer->getDevice(), hwnd);
	wibbleExtract = new WibbleCubeExtract(renderer->getDevice(), hwnd);
	horizontalShader = new HoriszontalBlurShader(renderer->getDevice(), hwnd);
	verticalShader = new VerticalBlurShader(renderer->getDevice(), hwnd);
	extractShader = new ExtractLightShader(renderer->getDevice(), hwnd);

	//load textures
	textureMgr->loadTexture("brick", L"res/brick1.dds");
	textureMgr->loadTexture("bunny", L"res/bunny.png");
	textureMgr->loadTexture("normal", L"res/NormalMap.png");

	int shadowmapWidth = 2048 * 2;
	int shadowmapHeight = 2048 *2;
	int sceneWidth = 100;
	int sceneHeight = 100;

	//setup light info
	shadowInfo_ = new shadowInfo;
	light[0] = new Light;
	light[0]->setAmbientColour(0.0f, 0.f, 0.f, 1.0f);
	light[0]->setDiffuseColour(0.0f, 6.f, 0.0f, 1.0f);
	light[0]->setDirection(1.0f, -0.2f, 0.7f);
	light[0]->setPosition(0.f, 0.f, -10.f);
	light[0]->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 100.f);

	light[1] = new Light;
	light[1]->setAmbientColour(0.0f, 0.0f, 0.0f, 1.0f);
	light[1]->setDiffuseColour(0.0f, 0.0f, 0.0f, 1.0f);
	light[1]->setDirection(1.0f, -1.f, 1.0f);
	light[1]->setPosition(-5.f, 1.f, -1.f);
	light[1]->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 100.f);
	//shadowInfo_->light[1]->generateViewMatrix();

	//setup ortho meshes
	screenOrtho = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight);
	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth / 4, screenHeight / 4, screenWidth / -4, screenHeight / -4);

	orthoMesh1 = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth / 4, screenHeight / 4, screenWidth / 4, screenHeight / -4);

	stopTime = true;

	//set up render textures
	shadowInfo_->shadowMap[0] = new RenderTexture(renderer->getDevice(), shadowmapWidth, shadowmapHeight, SCREEN_NEAR, SCREEN_DEPTH);
	shadowInfo_->shadowMap[1] = new RenderTexture(renderer->getDevice(), shadowmapWidth, shadowmapHeight, SCREEN_NEAR, SCREEN_DEPTH);
	horizontalBlurTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	verticalBlurTexture = new RenderTexture(renderer->getDevice(), screenWidth , screenHeight , SCREEN_NEAR, SCREEN_DEPTH);
	bloomTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	texture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	testTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);	
}

//renders thr brightest part of the geoometry to a render texture
void App1::extractLight()
{
	// Set the render target to be the render to texture and clear it
	testTexture->setRenderTarget(renderer->getDeviceContext());
	testTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);

	// Get matrices		
	camera->update();

	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();


	worldMatrix = XMMatrixTranslation(-50.f, 0.f, -10.f);

	// Render floor
	plane->sendData(renderer->getDeviceContext());
	extractShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("bricks"), light, brightThreshHold);
	extractShader->render(renderer->getDeviceContext(), plane->getIndexCount());
	
	worldMatrix = renderer->getWorldMatrix();
	// Render shape with simple lighting shader set.
	worldMatrix = XMMatrixTranslation(0.f, 7.f, 0.f);
	cube->sendData(renderer->getDeviceContext());
	extractShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("bunny"), light, brightThreshHold);
	extractShader->render(renderer->getDeviceContext(), cube->getIndexCount());
	
	//bymp sphere
	worldMatrix = XMMatrixTranslation(0.0f, 5.0f, 10.0f);
	sphere->sendData(renderer->getDeviceContext());
	sphereExtract->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("bunny"), textureMgr->getTexture("normalMap"), light, brightThreshHold);
	sphereExtract->render(renderer->getDeviceContext(), sphere->getIndexCount());

	//bibble cube
	worldMatrix = XMMatrixTranslation(-5.f, 7.f, 5.f);
	tesselatedCube->sendData(renderer->getDeviceContext());
	wibbleExtract->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("bunny"), light, brightThreshHold);
	wibbleExtract->render(renderer->getDeviceContext(), tesselatedCube->getIndexCount());		

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}




void App1::depthPass()
{
	//stop the time, moving object wont move if acive 
	if (!stopTime)
	{
		time += timer->getTime();
	}
	
	matrixInfo matrixInfo_;


	//need to loop twice so it is at the perspective of each light
	for (int i = 0; i < 2; i++)
	{
		//set up render textures 
		shadowInfo_->shadowMap[i]->setRenderTarget(renderer->getDeviceContext());
		shadowInfo_->shadowMap[i]->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);

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
		

		// Render model
		worldMatrix = renderer->getWorldMatrix();
		worldMatrix = XMMatrixTranslation(0.f, 7.f, 5.f);
		tesselatedCube->sendData(renderer->getDeviceContext());
		tessDepth->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix, time, amplitude, angularWave, angularFrequency, phaseShift, camera->getPosition());
		tessDepth->render(renderer->getDeviceContext(), tesselatedCube->getIndexCount());

		//normal cube
		worldMatrix = renderer->getWorldMatrix();
		worldMatrix = XMMatrixTranslation(0.f, 7.f, 0.f);
		cube->sendData(renderer->getDeviceContext());
		depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
		depthShader->render(renderer->getDeviceContext(), cube->getIndexCount());

		//sphere
		worldMatrix = XMMatrixTranslation(0.0f, 5.0f, 10.0f);
		sphere->sendData(renderer->getDeviceContext());
		sphereDepth->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
		sphereDepth->render(renderer->getDeviceContext(), sphere->getIndexCount());


		// Set back buffer as render target and reset view port.
		renderer->setBackBufferRenderTarget();
		renderer->resetViewport();
	}	
}

void App1::shadowPass()
{
	//set up render texture
	texture->setRenderTarget(renderer->getDeviceContext());
	texture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);

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

	// tesselatted cube
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(-5.f, 7.f, 5.f);
	tesselatedCube->sendData(renderer->getDeviceContext());
	tessShadow->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("bunny"),  shadowInfo_->shadowMap[0]->getShaderResourceView(), shadowInfo_->shadowMap[1]->getShaderResourceView(), light, time, amplitude, angularWave, angularFrequency, phaseShift, camera->getPosition());
	tessShadow->render(renderer->getDeviceContext(), tesselatedCube->getIndexCount());

	//normal cube
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(0.f, 7.f, 0.f);
	cube->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("bunny"), light, shadowInfo_->shadowMap[0]->getShaderResourceView(), shadowInfo_->shadowMap[1]->getShaderResourceView());
	shadowShader->render(renderer->getDeviceContext(), cube->getIndexCount());

	//sphere
	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(0.f, 7.0f, 20.f);
	sphere->sendData(renderer->getDeviceContext());
	sphereShadow->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("bunny"), light, shadowInfo_->shadowMap[0]->getShaderResourceView(), shadowInfo_->shadowMap[1]->getShaderResourceView(), textureMgr->getTexture("normal"));
	sphereShadow->render(renderer->getDeviceContext(), sphere->getIndexCount());


	worldMatrix = renderer->getWorldMatrix();
	worldMatrix = XMMatrixTranslation(0.0f, 5.0f, 10.0f);
	sphere->sendData(renderer->getDeviceContext());
	bumpShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture("bunny"), textureMgr->getTexture("normal"), light);
	bumpShader->render(renderer->getDeviceContext(), sphere->getIndexCount());


	renderer->setBackBufferRenderTarget();	

}

void App1::horizontalBlur()
{
	float screenX = float(horizontalBlurTexture->getTextureHeight());
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;
	horizontalBlurTexture->setRenderTarget(renderer->getDeviceContext());
	horizontalBlurTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);

	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	orthoMatrix = horizontalBlurTexture->getOrthoMatrix();

	// Render for Horizontal Blur
	renderer->setZBuffer(false);
	screenOrtho->sendData(renderer->getDeviceContext());
	horizontalShader->setShderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, testTexture->getShaderResourceView(), screenX, times);
	horizontalShader->render(renderer->getDeviceContext(), screenOrtho->getIndexCount());
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();

}

void App1::verticalBlur()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	float screenY = (float)verticalBlurTexture->getTextureHeight();
	verticalBlurTexture->setRenderTarget(renderer->getDeviceContext());
	verticalBlurTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 0.0f, 1.0f, 1.0f);

	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	// Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
	orthoMatrix = verticalBlurTexture->getOrthoMatrix();

	// Render for Vertical Blur
	renderer->setZBuffer(false);
	screenOrtho->sendData(renderer->getDeviceContext());
	verticalShader->setShderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, horizontalBlurTexture->getShaderResourceView(), screenY, times);
	verticalShader->render(renderer->getDeviceContext(), screenOrtho->getIndexCount());
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

void App1::bloomPass()
{
	extractLight();
	horizontalBlur();
	verticalBlur();
	combinePass();
}

void App1::combinePass()
{

	bloomTexture->setRenderTarget(renderer->getDeviceContext());
	bloomTexture->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 1.0f, 1.0f);

	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;
	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	// Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
	orthoMatrix = bloomTexture->getOrthoMatrix();

	
	// Render for Vertical Blur
	//renderer->setZBuffer(false);

	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();
	renderer->setZBuffer(false);
	screenOrtho->sendData(renderer->getDeviceContext());
	combine->setShderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, texture->getShaderResourceView(), verticalBlurTexture->getShaderResourceView());
	combine->render(renderer->getDeviceContext(), screenOrtho->getIndexCount());
	renderer->setZBuffer(true);

	renderer->setBackBufferRenderTarget();

}

void App1::finalPass()
{
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	renderer->setZBuffer(false);
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX orthoMatrix = renderer->getOrthoMatrix();  // ortho matrix for 2D rendering
	XMMATRIX orthoViewMatrix = camera->getOrthoViewMatrix();	// Default camera position for orthographic rendering

	//render the scene
	screenOrtho->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthoViewMatrix, orthoMatrix, bloomTexture->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), screenOrtho->getIndexCount());
	renderer->setZBuffer(true);



	worldMatrix = renderer->getWorldMatrix();
	XMMATRIX orthView = camera->getOrthoViewMatrix();
	XMMATRIX orthProj = renderer->getOrthoMatrix();

	//render shadow maps to smaller ortho meshes 
	renderer->setZBuffer(false);
	orthoMesh->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthView, orthProj, shadowInfo_->shadowMap[0]->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setZBuffer(true);

	worldMatrix = renderer->getWorldMatrix();
	renderer->setZBuffer(false);
	orthoMesh1->sendData(renderer->getDeviceContext());
	textureShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, orthView, orthProj, shadowInfo_->shadowMap[1]->getShaderResourceView());
	textureShader->render(renderer->getDeviceContext(), orthoMesh1->getIndexCount());
	renderer->setZBuffer(true);

	// Render GUI
	gui();

	// Present the rendered scene to the screen.
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

	//update light colour based on user input
	light[0]->setDiffuseColour(colour[0], colour[1], colour[2], colour[3]);
	light[1]->setDiffuseColour(colour1[0], colour1[1], colour1[2], colour1[3]);
	light[0]->setAmbientColour(colour1[0], colourAmbiant[1], colourAmbiant[2], colourAmbiant[3]);
	light[1]->setAmbientColour(colour1[0], colourAmbiant1[1], colourAmbiant1[2], colourAmbiant1[3]);

	return true;
}

bool App1::render()
{
	depthPass();
	shadowPass();
	bloomPass();
	renderer->setWireframeMode(false);
	finalPass();
	return true;
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	if (ImGui::CollapsingHeader("Vars"))
	{
		// Build UI
		ImGui::Text("FPS: %.2f", timer->getFPS());
		ImGui::Checkbox("Wireframe mode", &wireframeToggle);

		//if (ImGui::TreeNode("Angular"))
		//{
			ImGui::SliderFloat("amplitude[0]", &amplitude[0], 0, 5.0f);
			ImGui::SliderFloat("amplitude[1]", &amplitude[1], 0, 5.0f);
			ImGui::SliderFloat("angularWave[0]", &angularWave[0], 0, 20);
			ImGui::SliderFloat("angularWave[1]", &angularWave[1], 0, 20);
			
		//}

		ImGui::SliderFloat("angularFrequency[0]", &angularFrequency[0], 0, 20);
		ImGui::SliderFloat("angularFrequency[1]", &angularFrequency[1], 0, 20);

		ImGui::SliderFloat("phaseShift[0]", &phaseShift[0], 0, 5);
		ImGui::SliderFloat("phaseShift[1]", &phaseShift[1], 0, 5);

		ImGui::InputInt("numebr of blurs", &times);
		ImGui::SliderFloat("bloom Treshhold", &brightThreshHold, 0, 1);
		ImGui::Checkbox("Stop time", &stopTime);

		ImGui::ColorPicker4("Defuse1", colour);
		ImGui::ColorPicker4("Ambiant1", colourAmbiant);
		ImGui::ColorPicker4("Deffuse2", colour1);
		
		ImGui::ColorPicker4("Ambiant2", colourAmbiant1);

		//ImGui::TreePop();
	}

	// Render UI
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_FirstUseEver);
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

