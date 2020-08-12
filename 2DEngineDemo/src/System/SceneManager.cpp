#include "SceneManager.h"

#include "../Scene/TitleScene.h"

SceneManager::SceneManager(KeyboardInput& sceneInput):sceneInput_(sceneInput)
{
	scenes_.emplace_back(new TitleScene(*this,sceneInput_));
}

void SceneManager::ProcessInput()
{
	scenes_.back()->ProcessInput();
}

void SceneManager::Update(const float& deltaTime)
{
	scenes_.back()->Update(deltaTime);
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
	scenes_.back() = std::move(scene);
}

void SceneManager::ClearScene()
{
	scenes_.clear();
}

void SceneManager::ResetScene(active_scene scene)
{
	ClearScene();
	scenes_.emplace_back(std::move(scene));
}

void SceneManager::PushScene(active_scene scene)
{
	scenes_.emplace_back(std::move(scene));
}

void SceneManager::PopScene()
{
	scenes_.pop_back();
}
