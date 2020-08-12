#include "BaseScene.h"

#include "../System/SceneManager.h"
#include "../Input/KeyboardInput.h"


BaseScene::BaseScene(SceneManager& sceneMng, KeyboardInput& sceneInput):
	sceneMng_(sceneMng),sceneInput_(sceneInput)
{
}
