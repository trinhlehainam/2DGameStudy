#include "PauseScene.h"

#include <DxLib.h>

#include "../Constant.h"
#include "SceneManager.h"
#include "../Input/KeyboardInput.h"

PauseScene::PauseScene(SceneManager& sceneMng, KeyboardInput& sceneInput):BaseScene(sceneMng,sceneInput)
{
}

PauseScene::~PauseScene()
{
}

void PauseScene::ProcessInput()
{
	if (sceneInput_.IsTriggered("pause"))
	{
		sceneMng_.PopScene();
	}
	
}

void PauseScene::Update(const float& deltaTime)
{

}

void PauseScene::Render()
{
	DxLib::DrawString(300, 300, L"Pause Scene is Active", 0xffffff);
}
