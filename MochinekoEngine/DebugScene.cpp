#include "DebugScene.h"
#include "ObjectManager.h"
#include "FBX.h"
#include "SoundManager.h"
#include "Image.h"
#include "ImageManager.h"
#include "DX3DManager.h"
#include "VideoObject.h"
#include "../Player.h"
#include "../MessageManager.h"
#include "ModelManager.h"
#include "../UserResource.h"
#include "../MapManager.h"

using namespace DX3DManager;

void DebugScene::Init() {
	MessageManager::Init();

	FBX* grid = new FBX("MochinekoEngine/Asset/grid.fbx", false, true);
	grid->SetTag("Grid");
	ObjectManager::AddObject(grid);

	FBX* gizmo = new FBX("test.fbx", false, false);
	gizmo->SetTag("Gizmo");
	//gizmo->AddChild(grid);
	ObjectManager::AddObject(gizmo);

	ObjectManager::AddObject(new Player());

	//VideoObject* video = new VideoObject("a.mp4");
	//ObjectManager::AddObject(video);

	MapManager::Init();
}

void DebugScene::Update() {
}

void DebugScene::Draw() {
	MessageManager::Draw();
	MapManager::Draw();
}
