#pragma once

#include <memory>
#include <deque>

#include "BaseScene.h"

using active_scene = std::unique_ptr<BaseScene>;
class Engine;
class KeyboardInput;

class SceneManager
{
	friend Engine;
private:
	
	std::deque<active_scene> scenes_;
	KeyboardInput& sceneInput_;

	SceneManager(KeyboardInput&);
	SceneManager(const SceneManager&) = delete;
	SceneManager(SceneManager&&) = delete;
	SceneManager& operator = (const SceneManager&) = delete;
	SceneManager& operator = (SceneManager&&) = default;

public:
	void ProcessInput();
	void Update(const float& deltaTime);
	void Render();
	void ChangeScene(active_scene scene);
	void ClearScene();
	void PushScene(active_scene scene);
	void PopScene();
	~SceneManager() = default;
};

