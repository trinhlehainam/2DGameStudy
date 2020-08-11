#include "SceneManager.h"

#include "TitleScene.h"

SceneManager::SceneManager(KeyboardInput& sceneInput):sceneInput_(sceneInput)
{
	scenes_.emplace_front(new TitleScene(*this,sceneInput_));
}

void SceneManager::ProcessInput()
{
	scenes_.front()->ProcessInput();
}

void SceneManager::Update(const float& deltaTime)
{
	scenes_.front()->Update(deltaTime);
}

void SceneManager::Render()
{
	for (auto& scene : scenes_)
	{
		scene->Render();
	}
}

void SceneManager::ChangeScene(active_scene scene)
{
	scenes_.front() = std::move(scene);
}

void SceneManager::ClearScene()
{
	scenes_.clear();
}

void SceneManager::PushScene(active_scene scene)
{
	scenes_.emplace_front(std::move(scene));
}

void SceneManager::PopScene()
{
	scenes_.pop_front();
}
