#include "BaseScene.h"

#include "SceneManager.h"
#include "../Input/KeyboardInput.h"


BaseScene::BaseScene(SceneManager& sceneMng, KeyboardInput& sceneInput):
	sceneMng_(sceneMng),sceneInput_(sceneInput)
{
}
