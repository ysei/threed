#include <engine/Engine.h>
#include <engine/SceneNode.h>
#include <engine/Geometry.h>
#include <engine/Camera.h>
#include <Math/Math.h>
#include <renderer/Renderer.h>
#include <resource/ResourceManager.h>
#include <assert.h>

namespace Engine {

// the one and only game engine
Engine *gEngine;

Engine::Engine()
:	mRenderer(0),
	mResources(0),
	mMainCamera(0)
{
	mNodeTree = new SceneNode;
	mResources = new ResourceManager;

	assert(!gEngine);
	gEngine = this;
}

Engine::~Engine()
{
	delete mNodeTree;
	gEngine = 0;
}

void Engine::SetRenderer(Renderer &r)
{
	assert(!mRenderer);
	mRenderer = &r;
}

static Spatial *testSpatial;

int Engine::InnerLoop()
{
	mRenderer->StartFrame();

//	mNodeTree->Move(Math::Vector3(0, 0, 0.001f));
//	mNodeTree->Rotate(Math::Vector3(0.01f, 0.01f, 0.01f));
	testSpatial->Rotate(Math::Vector3(0.00f, 0.01f, 0.00f));

	Math::Matrix4x4 proj;
	proj.SetProjectionPerspective(Math::DegreeToRadians(45.0f * mMainCamera->GetZoom()), (float)(mRenderer->GetWindowWidth())/(float)(mRenderer->GetWindowHeight()), 1.0f, 1000.0f);
//	std::cout << proj << std::endl;
//	proj.SetProjectionOrtho(0, (float)(mRenderer->GetWindowWidth()), 0, (float)(mRenderer->GetWindowHeight()), 1.0f, 100.0f);
	mRenderer->SetProjMatrix(proj);

	// set up the main camera
	mMainCamera->Render(mRenderer);

	// update the scene graph
	mNodeTree->UpdateTransform(false);
	mNodeTree->Render(mRenderer);

	mRenderer->EndFrame();

	return 0;
}

void Engine::SetupDefaultScene()
{
	SceneNode *node;
	Spatial *spatial;

	Resource *r;
	// plane
	r = mResources->GetResource("plane", RT_OBJECT);
	Spatial *plane = Geometry::BuildFromResource(r);
	assert(plane);
	r->RemoveRef();

	plane->Move(Math::Vector3(0, 0, -10.0f));
	plane->Rotate(Math::Vector3(-1, 1, 0));
	mNodeTree->AddChild(plane);

	r = mResources->GetResource("jess2", RT_OBJECT);
	spatial = Geometry::BuildFromResource(r);
	assert(spatial);
	r->RemoveRef();

	assert(spatial);
	spatial->Scale(5.0f);
	mNodeTree->AddChild(spatial);

	testSpatial = spatial;

	r = mResources->GetResource("plane", RT_OBJECT);
	spatial = Geometry::BuildFromResource(r);
	assert(spatial);
	r->RemoveRef();

	spatial->Scale(1.5f);
	spatial->Move(Math::Vector3(-5, 3.0f, -20.0f));
	spatial->Rotate(Math::Vector3(1, 0, -10));
	mNodeTree->AddChild(spatial);

	r = mResources->GetResource("map", RT_OBJECT);
	spatial = Geometry::BuildFromResource(r);
	assert(spatial);
	r->RemoveRef();

	assert(spatial);
//	spatial->Scale(.1f);
	spatial->Rotate(Math::Vector3(Math::DegreeToRadians(-90), 0, 0));
	mNodeTree->AddChild(spatial);

	mNodeTree->Move(Math::Vector3(0, 0, 1.0f));

	// create a camera to look through
	mMainCamera = new Camera();
	mMainCamera->SetPos(Math::Vector3(2, 2, 5));

	r = mResources->GetResource("dude", RT_OBJECT);
	spatial = Geometry::BuildFromResource(r);
	spatial->Move(Math::Vector3(1,1,1));
	mNodeTree->AddChild(spatial);

	mResources->DumpResources();
}

}
